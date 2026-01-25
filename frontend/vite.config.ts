import { defineConfig } from "vite";
import react from "@vitejs/plugin-react";

export default defineConfig({
  plugins: [react()],
  resolve: {
    dedupe: ["react", "react-dom"],
  },
  server: {
    proxy: {
      "/api": {
        target: "http://localhost:3000",
        changeOrigin: true,
        configure: (proxy, options) => {
          const targetUrl =
            typeof options.target === "string"
              ? options.target
              : `${options.target?.protocol}//${options.target?.host}`;

          proxy.on("proxyReq", (proxyReq, req) => {
            console.log("🔵 [Vite Proxy] Request:", req.method, req.url);
            console.log("   → Forwarding to:", targetUrl + req.url);
          });

          proxy.on("proxyRes", (proxyRes, req) => {
            console.log(
              "🟢 [Vite Proxy] Response:",
              proxyRes.statusCode,
              req.url,
            );
          });

          proxy.on("error", (err) => {
            console.log("🔴 [Vite Proxy] Error:", err.message);
          });
        },
      },
    },
  },
});
