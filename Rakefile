require 'bundler'
require 'rake/testtask'
require 'rake/extensiontask'

Bundler::GemHelper.install_tasks

Rake::TestTask.new do |t|
  t.libs = ["lib"]
  # t.warning = true
  t.verbose = true
  t.test_files = FileList['test/**/*.rb'].reject do |file|
    file.end_with?('_lib.rb') || file.include?('/_lib/')
  end
end

Rake::ExtensionTask.new "configatron_ext"

task :default => :test
task :test => :compile
