# {{PROXYME}}
{{#PROXY}}
Acquire::http::Proxy "http://{{#PROXY_AUTH}}{{PROXY_USER}}:{{PROXY_PWD}}@{{/PROXY_AUTH}}{{PROXY_HOST}}:{{PROXY_PORT}}";
Acquire::https::Proxy "http://{{#PROXY_AUTH}}{{PROXY_USER}}:{{PROXY_PWD}}@{{/PROXY_AUTH}}{{PROXY_HOST}}:{{PROXY_PORT}}";
Acquire::ftp::Proxy "http://{{#PROXY_AUTH}}{{PROXY_USER}}:{{PROXY_PWD}}@{{/PROXY_AUTH}}{{PROXY_HOST}}:{{PROXY_PORT}}";
{{/PROXY}}
