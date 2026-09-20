---
layout: page
title: Atelier
permalink: /atelier/
---

Atelier is my framework for experimenting with games and game-playing agents.

{% for article in site.atelier %}
- [{{ article.title }}]({{ article.url | relative_url }})
{% endfor %}