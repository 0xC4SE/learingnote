let mapleader=" "
set encoding=utf-8
set fileencoding=utf-8
source $VIMRUNTIME/delmenu.vim
source $VIMRUNTIME/menu.vim
language messages zh_CN.utf-8
" set number
set relativenumber
"colorscheme torte 
colorscheme atom-dark-256

syntax enable 
syntax on
let g:winManagerWindowLayout='FileExplorer|TagList' 
nmap wm :WMToggle<cr>
let Tlist_Show_One_File=1 
let Tlist_Exit_OnlyWindow=1
set lines=35 columns=131
winpos 600 300
set wildmenu
set hlsearch
set incsearch
set backspace=2
set cursorline
set showcmd
set ignorecase
set smartcase
set scrolloff=7
set guioptions-=m
set guioptions-=T
set tabstop=4
set expandtab
set shiftwidth=4
set softtabstop=4
set autoindent


noremap j h
noremap k j
noremap i k
noremap h i
noremap H I
map I 5i
map K 5k
map J 3j
map L 3l
" 打开一个新的标签页
nmap <LEADER>et :tabe<CR>

nmap <leader>ec :e! C:\
nmap <leader>ed :e! D:\
nmap <leader>ee :e! E:\
nmap <leader>ef :e! F:\
" 切换标签页l是往右j是往左
nmap <LEADER>l :tabn<CR>
nmap <LEADER>j :tabp<CR>
" 插入模式下的向左右移动复制删除和粘贴
imap <C-j> <Left>
imap <C-l> <Right>
imap <C-y><C-y> <ESC>yya
imap <C-p> <ESC>pa
imap <C-d><C-d> <ESC>dda
" jj是返回的映射 
imap jj <ESC>
vmap jj <ESC>
" s设置为nop以便快捷键的加入
map s <nop>
map S :w<CR>
map sss :q!<CR>
map sjl :source %<CR>
" 将所有成对符号光标居中
imap '3' ''<right>,''<right>,''<left><left><left><left><left><left>
imap '2' ''<right>,''<left><left><left>
imap "3" ""<right>,""<right>,""<left><left><left><left><left><left>
imap "2" ""<right>,""<left><left><left>
imap {} {}<left>
imap () ()<left>
imap <> <><left>
imap '' ''<left>
imap "" ""<left>
imap [] []<left>
"===================
"复制系统粘贴板到vim粘贴板
nmap cp :set paste<CR>"+p:set nopaste<CR>
" 打开分屏空格加ww是竖向分屏空格加w是左右分屏
map <LEADER>ww :split<CR>
map <LEADER>w :vsplit<CR>
" res是设置分屏大小的改变up就是增大down就是减小同理左右也一样
nmap <LEADER><up> :res +5<CR>
nmap <LEADER><down> :res -5<CR>
nmap <LEADER><left> :vertical resize-5<CR>
nmap <LEADER><right> :vertical resize+5<CR>
    
nmap <leader><CR> :e ~/_vimrc<CR>
"vim-plug的使用
call plug#begin('C:\Users\DELL\.vim\plugged')
Plug 'vim-airline/vim-airline'
Plug 'majutsushi/tagbar', { 'on': 'TagbarOpenAutoClose' }
Plug 'Yggdroot/LeaderF', { 'do': '.\install.bat' }
Plug 'neoclide/coc.nvim', {'branch': 'release'}
Plug 'neoclide/coc.nvim', {'do': 'yarn install --frozen-lockfile'}
call plug#end()


" easyalign使用方法为块选中ga加以之对其的操作符
xmap ga <Plug>(EasyAlign)
nmap ga <Plug>(EasyAlign)

" tagbar配置
map <silent> T :TagbarOpenAutoClose<CR>
nmap <F8> :Tagbar<CR>

" leaderF的配置
" <C-j>在结果窗口中向下移动光标<C-K>在结果窗口中向上移动光标<C-R>在模糊搜索模式和正则表达式模式之间切换
" <C-F>在全路径搜索模式和仅名称搜索模式之间切换<Tab>切换到普通模式
" <leader>f是查找当前盘符内所有文件
" fb是查找buffer
noremap <leader>fb :<C-U><C-R>=printf("Leaderf buffer %s", "")<CR><CR>
" fm最近打开过的文件
noremap <leader>fm :<C-U><C-R>=printf("Leaderf mru %s", "")<CR><CR>
" ft查看所有变量和函数以及是否全局
noremap <leader>ft :<C-U><C-R>=printf("Leaderf bufTag %s", "")<CR><CR>
" fl查找所有行内的东西
noremap <leader>fl :<C-U><C-R>=printf("Leaderf line %s", "")<CR><CR>
noremap <leader>fo :<C-U><C-R>=printf("Leaderf! gtags --recall %s", "")<CR><CR>
" 查找所有的function
noremap <leader>fn :LeaderfFunction<CR>
noremap <leader>fna :LeaderfFunctionAll<CR>
" 查看leaderf可执行的帮助文档
noremap <leader>fp :Leaderf self<CR>
let g:Lf_IgnoreCurrentBufferName = 1

" coc.nvim
" ========
let g:coc_global_extentions = ['coc-json','coc-vimlsp']
set updatetime=100
inoremap <silent><expr> <TAB>
      \ pumvisible() ? "\<C-n>" :
      \ <SID>check_back_space() ? "\<TAB>" :
      \ coc#refresh()
inoremap <expr><S-TAB> pumvisible() ? "\<C-p>" : "\<C-h>"

function! s:check_back_space() abort
  let col = col('.') - 1
  return !col || getline('.')[col - 1]  =~# '\s'
endfunction

if has('nvim')
  inoremap <silent><expr> <C-space> coc#refresh()
else
  inoremap <silent><expr> <C-o> coc#refresh()
endif

if exists('*complete_info')
  inoremap <expr> <cr> complete_info()["selected"] != "-1" ? "\<C-y>" : "\<C-g>u\<CR>"
else
  inoremap <expr> <cr> pumvisible() ? "\<C-y>" : "\<C-g>u\<CR>"
endif
nmap <silent> <leader>- <Plug>(coc-diagnostic-prev)
nmap <silent> <leader>= <Plug>(coc-diagnostic-next)

nmap <silent> gd <Plug>(coc-definition)
nmap <silent> gy <Plug>(coc-type-definition)
nmap <silent> gi <Plug>(coc-implementation)
nmap <silent> gr <Plug>(coc-references)
nnoremap <silent> <leader>hp :call <SID>show_documentation()<CR>

autocmd CursorHold * silent call CocActionAsync('highlight')


