project('tiny-vm', 'c')
args = ['-Wall', '-Wextra', '-Wshadow', '-Werror', '-O3', '-g']

vm_files = ['src/main.c', 'src/vm.c']
executable('vm', vm_files, c_args : args)

as_files = ['src/as.c']
executable('as', as_files, c_args : args)
