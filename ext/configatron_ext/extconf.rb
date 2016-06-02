require 'mkmf'

$CFLAGS << ' -g3 -ggdb' # rubocop:disable Style/GlobalVars

create_makefile('configatron_ext')
