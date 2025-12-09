RubyVM::YJIT.enable

require "camlib"
require "concurrent"
require "concurrent-edge"
require "open3"

target_fps = 30.0
delay_ms = 1000.0 / target_fps

frames = Concurrent::Channel.new(capacity: 5)
done = Concurrent::Channel.new(capacity: 2)

run_time_s = 5

cam = Camlib::Webcam.acquire!
puts RubyVM::YJIT.enabled?

start_time = Time.now
last_frame_time = start_time

video_size = "#{cam.frame_width}x#{cam.frame_height}"
pixel_format = "bgr24"
cam2 = Camlib::Webcam.acquire!

Concurrent::Channel.go do 
  puts "starting camera"
  loop do
    current_time = Time.now
    elapsed_time_ms = (current_time - last_frame_time) * 1000

    if elapsed_time_ms >= delay_ms
      frames << cam.read_frame.force_encoding(Encoding::ASCII_8BIT)
      last_frame_time = current_time
    end

    if Time.now - start_time > run_time_s
      break
    end
  end
  puts "frames done"
  frames << :end
end

Concurrent::Channel.go do
  puts "starting processing"
  frame_count = 0
  cmd = "ffmpeg -y -f rawvideo -pixel_format #{pixel_format} -video_size #{video_size} -framerate #{cam.frame_rate} -i pipe: -c:v libx264 -preset veryfast -f mp4 test.mp4"
    puts "opening process"
    Open3.popen2e(cmd) do |stdin, stdout_stderr, wait_thread|
      puts "opened process"

      output_thread = Thread.new do
        puts "printing stdout"
        stdout_stderr.each { |line| puts line }
      end

      loop do
        puts frame_count
        frame = frames.take
        if frame == :end
          puts "frame processing ended"
          break
        end

        stdin.write(frame)
        File.binwrite("/tmp/#{frame_count}.bin", frame)

        frame_count += 1
      end

      stdin.close
      output_thread.join
    end

    puts "frame_count put to done"
    done << frame_count
end


frame_count = done.take

puts cam2.read_frame.size

# puts "Target: #{target_fps * run_time_s}, Achieved: #{frames.size} (#{frames.size / run_time_s} fps)"
puts "Target: #{target_fps * run_time_s}, Achieved: #{frame_count}"


