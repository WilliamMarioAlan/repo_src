-- init.lua
-- 主配置入口文件，初始化基础设置和插件管理器

-- 加载基础选项配置（比如行号、缩进等）
require("core.options")

-- 加载快捷键映射配置（比如 <leader>f 格式化等）
require("core.mappings")


-- 设置插件管理器 lazy.nvim 的路径
local lazypath = vim.fn.stdpath("data") .. "/lazy/lazy.nvim"
if not vim.loop.fs_stat(lazypath) then
  vim.notify("lazy.nvim not found! 请检查是否已安装", vim.log.levels.ERROR)
  return
end
vim.opt.rtp:prepend(lazypath)

-- 加载插件系统
require("lazy").setup("plugins")

