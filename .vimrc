"let g:ale_c_cc_options = '-std=c99 -Wall -iquote ~/projects/MrsWatson/source'
let g:ale_c_cc_options = '-std=c99 -Wall -iquote .'

"let g:ale_c_clangtidy_options = '-std=c99 -Wall -iquote ~/projects/MrsWatson/source'
let g:ale_c_clangtidy_options = '-std=c99 -Wall -iquote .'

"let g:ale_c_clangcheck_options = '--extra-arg=-std=c99 --extra-arg=-Wall --extra-arg=-iquote --extra-arg=~/projects/MrsWatson/source'
let g:ale_c_clangcheck_options = '--extra-arg=-std=c99 --extra-arg=-Wall --extra-arg=-iquote --extra-arg=.'

"let g:ale_c_build_dir = '../build'

autocmd VimEnter * :echo "local vimrc loaded! 😺"
