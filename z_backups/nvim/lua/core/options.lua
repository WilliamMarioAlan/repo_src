-- core/options.lua
-- 设置 Neovim 的基础行为参数

local opt = vim.opt

-- 显示行号
opt.number = true
opt.relativenumber = true

-- 缩进相关
opt.tabstop = 4         -- tab 显示宽度
opt.shiftwidth = 4      -- 自动缩进宽度
opt.expandtab = true    -- tab 替换为空格
opt.smartindent = true  -- 智能缩进

-- 搜索增强
opt.ignorecase = true
opt.smartcase = true

-- UI 改进
opt.termguicolors = true
opt.cursorline = true
opt.signcolumn = "yes"

-- 文件行为
opt.undofile = true     -- 撤销历史持久化
opt.swapfile = false    -- 禁用 swap 文件
opt.backup = false      -- 禁用 backup 文件

-- 滚动时上下保留行数
opt.scrolloff = 8
opt.sidescrolloff = 8

