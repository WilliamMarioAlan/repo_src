-- core/mappings.lua
-- 快捷键设置模块

local map = vim.keymap.set
local opts = { noremap = true, silent = true }

-- 设置 <Space> 为 leader 键
vim.g.mapleader = " "

-- 常用功能
map("n", "<leader>w", ":w<CR>", opts)         -- 保存
map("n", "<leader>q", ":q<CR>", opts)         -- 退出
map("n", "<leader>f", ":lua vim.lsp.buf.format()<CR>", opts)  -- 格式化（后续 LSP 会生效）

-- LSP 跳转（预设）
map("n", "gd", ":lua vim.lsp.buf.definition()<CR>", opts)     -- 跳转到定义
map("n", "gr", ":lua vim.lsp.buf.references()<CR>", opts)     -- 查找引用



