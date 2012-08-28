<?xml version="1.0" encoding="UTF-8"?>
<settings xmlns="http://maven.apache.org/SETTINGS/1.0.0"
          xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
          xsi:schemaLocation="http://maven.apache.org/SETTINGS/1.0.0 http://maven.apache.org/xsd/settings-1.0.0.xsd">

  <proxies>
<!--{{PROXYME}} -->
{{#PROXY}}
   <proxy>
      <active>true</active>
      <protocol>http</protocol>
      <host>{{PROXY_HOST}}</host>
      <port>{{PROXY_PORT}}</port>
      {{#PROXY_AUTH}}
      <username>{{PROXY_USER}}</username>
      <password>{{PROXY_PWD}}</password>
      {{/PROXY_AUTH}}
    </proxy>
{{/PROXY}}
  </proxies>
</settings>
