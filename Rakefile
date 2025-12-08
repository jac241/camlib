# frozen_string_literal: true

require "bundler/gem_tasks"
require "rake/clean"
require "fileutils"
require "rspec/core/rake_task"

task default: %i[compile test]

RSpec::Core::RakeTask.new(:test) do |t|
  t.pattern = "spec/**/*_spec.rb"
end

# Make the test task depend on compile so it rebuilds if needed
task test: :compile

task :compile do
  # Create a dedicated build directory within ext/
  # We use 'gembuild' to avoid conflicts with IDEs (like CLion) that might use 'build'
  build_dir = File.join('ext', 'gembuild')
  FileUtils.mkdir_p build_dir
  
  Dir.chdir(build_dir) do
    # Run cmake from the build directory, pointing to the parent (ext) for source
    sh "cmake .."
    sh "make"
  end

  # Copy the built artifact from the build_dir to the lib/ directory
  # Note: The extension matches the platform (e.g. .bundle on macOS, .so on Linux)
  # We glob to find it.
  built_lib_glob = File.join(build_dir, "*.{bundle,so,dylib}")
  built_lib = Dir[built_lib_glob].first
  
  if built_lib
    FileUtils.cp built_lib, 'lib/'
    puts "Copied #{built_lib} to lib/"
    
    # Create a marker file to tell Bundler that the extension is built
    # This suppresses the "extensions are not built" warning when running bin/console
    FileUtils.touch 'lib/gem.build_complete'
    puts "Created lib/gem.build_complete to suppress Bundler warnings"
  else
    raise "Error: No built extension found in #{build_dir}"
  end
end

# Define clean tasks
# We include the marker file and the build directory
CLEAN.include('ext/gembuild', 'lib/*.bundle', 'lib/*.so', 'lib/gem.build_complete')
task :clean do
  Rake::Task['clean'].invoke # Invoke the default clean task defined by rake/clean
end
