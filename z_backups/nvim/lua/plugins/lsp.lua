-- plugins/lsp.lua
-- LSP 支持，主要是 clangd（C/C++）

return {
  {
    "neovim/nvim-lspconfig",
    event = { "BufReadPre", "BufNewFile" },
    config = function()
      local lspconfig = require("lspconfig")

      -- 启用 clangd（C/C++ LSP）
      lspconfig.clangd.setup({
        cmd = { "clangd" }, -- 确保系统中已安装
        filetypes = { "c", "cpp", "objc", "objcpp" },
        root_dir = lspconfig.util.root_pattern("compile_commands.json", ".git"),
      })
    end
  }
}

