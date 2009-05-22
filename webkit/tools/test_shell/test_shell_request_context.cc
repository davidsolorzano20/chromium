// Copyright (c) 2006-2008 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "webkit/tools/test_shell/test_shell_request_context.h"

#include "net/base/cookie_monster.h"
#include "net/proxy/proxy_service.h"
#include "webkit/glue/webkit_glue.h"

TestShellRequestContext::TestShellRequestContext() {
  Init(std::wstring(), net::HttpCache::NORMAL, false);
}

TestShellRequestContext::TestShellRequestContext(
    const std::wstring& cache_path,
    net::HttpCache::Mode cache_mode,
    bool no_proxy) {
  Init(cache_path, cache_mode, no_proxy);
}

void TestShellRequestContext::Init(
    const std::wstring& cache_path,
    net::HttpCache::Mode cache_mode,
    bool no_proxy) {
  cookie_store_ = new net::CookieMonster();

  // hard-code A-L and A-C for test shells
  accept_language_ = "en-us,en";
  accept_charset_ = "iso-8859-1,*,utf-8";

  net::ProxyConfig proxy_config;
  proxy_service_ = net::ProxyService::Create(no_proxy ? &proxy_config : NULL,
                                             false, NULL, NULL);

  net::HttpCache *cache;
  if (cache_path.empty()) {
    cache = new net::HttpCache(proxy_service_, 0);
  } else {
    cache = new net::HttpCache(proxy_service_, cache_path, 0);
  }
  cache->set_mode(cache_mode);
  http_transaction_factory_ = cache;
}

TestShellRequestContext::~TestShellRequestContext() {
  delete cookie_store_;
  delete http_transaction_factory_;
  delete proxy_service_;
}

const std::string& TestShellRequestContext::GetUserAgent(
    const GURL& url) const {
  return webkit_glue::GetUserAgent(url);
}
