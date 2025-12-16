# frozen_string_literal: true

require_relative "lib/camlib/version"

Gem::Specification.new do |spec|
  spec.name = "camlib"
  spec.version = Camlib::VERSION
  spec.authors = [ "jac241" ]
  spec.email = [ "JamesCastiglione241@gmail.com" ]

  spec.summary = "Write a short summary, because RubyGems requires one."
  spec.description = "Write a longer description or delete this line."
  spec.homepage = "https://jac241.github.io"
  spec.required_ruby_version = ">= 3.1.0"

  # spec.metadata["allowed_push_host"] = "TODO: Set to your gem server 'https://example.com'"

  spec.metadata["homepage_uri"] = spec.homepage
  spec.metadata["source_code_uri"] = "https://github.com/jac241"

  # Specify which files should be added to the gem when it is released.
  # The `git ls-files -z` loads the files in the RubyGem that have been added into git.
  # gemspec = File.basename(__FILE__)

  spec.files = Dir["*.{md,txt}", "{ext,lib}/**/*"]

  spec.bindir = "exe"
  spec.executables = spec.files.grep(%r{\Aexe/}) { |f| File.basename(f) }
  spec.require_paths = [ "lib" ]
  spec.extensions = [ "ext/CMakeLists.txt" ]
  spec.add_runtime_dependency('rice')
  spec.add_development_dependency("rspec")
  spec.add_development_dependency("concurrent-ruby")
  spec.add_development_dependency("concurrent-ruby-edge")

  # Uncomment to register a new dependency of your gem
  # spec.add_dependency "example-gem", "~> 1.0"

  # For more information and examples about making a new gem, check out our
  # guide at: https://bundler.io/guides/creating_gem.html
end
