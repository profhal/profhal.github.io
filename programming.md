---
layout: page
title: Programming
permalink: /programming/
---

## Progamming Related Articles

Articles I have written to support my classes.

## General

{% assign general_articles = site.programming | where: "category", "general" %}
{% for article in general_articles %}
- [{{ article.title }}]({{ article.url | relative_url }})
{% endfor %}

## C++

{% assign cpp_articles = site.programming | where: "category", "cpp" %}
{% for article in cpp_articles %}
- [{{ article.title }}]({{ article.url | relative_url }})
{% endfor %}
