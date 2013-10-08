autocmd BufEnter wscript set sw=4 ts=4 sta et fo=croql
autocmd BufLeave wscript set noet

let g:syntastic_mode_map = {
			\ "mode": "active",
			\ "active_filetypes": [],
			\ "passive_filetypes": []}

let g:syntastic_c_checkers = ['gcc']
let g:syntastic_c_compiler_options = "-std=c99 -D_GNU_SOURCE -fms-extensions"
let g:syntastic_c_include_dirs = [
			\ "include",
			\ "third-party",
			\ "third-party/gl3w",
			\ "third-party/libuv/include",
			\ "build/src",
			\ "build"]

set path+=include,third-party,third-party/gl3w,build/src,src,third-party/libuv/include
