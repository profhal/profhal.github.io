---
layout: page
title: Programming
permalink: /programming/
---

## Progamming Related Articles

Articles I have written to support my classes.

{% for article in site.programming %}
- [{{ article.title }}]({{ article.url | relative_url }})
{% endfor %}