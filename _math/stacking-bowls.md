---
layout: page
title: Stacking Bowls
---

While putting dishes away, a question occurred to me. I have 12 bowls in 6 colors (2 each). The cupboard height limits stacks of bowls to 4. So, there are three stacks. The question was, "Can I create stacks where no stack has a repeat color?"

It turns out in my case, yes. But what about the more general case? Let's formalize the above:

**Question:** Given $b$ bowls in $c$ colors, with an equal number of each color, can they be divided into $s$ equal-sized stacks with no repeated color?

<div class="vspace"></div>

We could prove answer this question directly, but before doing so, let's look more closely at the assumptions as we can do a little better.

The original problem assumes that every color occurs the same number of times and that every stack contains the same number of bowls. Neither assumption is necessary. What really matters is how many bowls there are of the most frequently occurring color and whether there are enough bowls to put at least one in each stack.

This leads to a more general result, Theorem 1.


<div class="vspace"></div>

**Theorem 1.** Suppose we have $b>0$ bowls in $c$ colors, where $1\le c\le b$, and let $n_{i}$ be the number of bowls of color $i$ where $0 \le i < c$.  Suppose we want to organize the bowls into $s$ nonempty stacks such that no stack contains a repeated color. It is possible to create such stacks if

$$
\max_{0\le i\le c-1} n_i\le s\le b.
$$

**Proof:** 

Suppose

$$
\max_{0\le i\le c-1} n_i\le s\le b.
$$

We have two things to show: each stack is non-empty and no stack has a repeated color. Since we need to demonstrate we *can* create the stacks, we explicitly describe the construction.

Start by listing the colors, $C_0, C_1, ... C_{c-1}$ and the stacks $S_0, S_1, ... S_{s-1}$. We will create the stacks in the following way: 

Start with color $C_0$. Place one bowl of color $C_0$ on consecutive stacks until you run out of bowls. Since there are $n_0$ bowls of color $C_0$ and $n_0 \le s$, the first distribution will look like

$$
\begin{array}{c|cccc}
\text{color} & C_0 & C_0 & ... & C_0 \\
\text{stack} & S_0 & S_1 & ... & S_{n_0 - 1} \\
\end{array}
$$

We then begin distributing the bowls of color $C_1$ in the same way:

$$
\begin{array}{c|cccccccccccc}
\text{color} & C_0 & C_0 & ... & C_0         &  C_1                  & ... & C_1                  & ... & C_1 \\
\text{stack} & S_0 & S_1 & ... & S_{n_0 - 1} &  S_{(n_0)\mod{s}} & ... & S_{(n_0 + 2)\mod{s}} & ... & S_{(n_0 + n_1 - 1)\mod{s}}\\
\end{array}
$$

If $n_0 + n_1 \le s$, we have covered the first $n_0 + n_1$ stacks with one bowl and the remaining $s - (n_0 + n_1)$ stacks are empty. If $n_0 + n_1 > s$, then we would wrap back to $S_0$ and continue distributing. Since $n_i \le s$, we will never place two bowls of the same color on the same stack. This means that no stack will ever have two bowls of the same color.

This leaves us to show that no stack is empty. This is due to our construction. If $b \ge s$ and we are distributing the bowls one at a time in a cyclical way, every stack will have at least one bowl. That is, since $b\ge s$, at least $s$ bowls are distributed. The first $s$ bowls are placed in $S_0,S_1,\ldots,S_{s-1}$, respectively. Thus every stack contains at least one bowl. $\qquad \square$


**Example:** Let's look at our initial situation. We have 12 bowls and 6 colors of 2 bowls each. Let's call the colors $A$, $B$, $C$, $D$, $E$, and $F$.  We want to create 3 stacks with no repeat colors. The construction would go as follows:

Distribute bowls of color $A$:

```
S_0 = {A}
S_1 = {A}
S_2 = {}
```

Then bowls of color $B$, which picks up with stack $S_2$ and wraps back to stack $S_0$:

```
S_0 = {A, B}
S_1 = {A}
S_2 = {B}
```

We continue through the remaining colors until we have

```
S_0 = {A, B, D, E}
S_1 = {A, C, D, F}
S_2 = {B, C, E, F}
```

<div class="vspace"></div>

One last observation: We have shown the conditions are sufficient to reach the conclusion, but it turns out they are also necessary. That is, we can strengthen this result:

**Theorem 2.** Suppose we have $b>0$ bowls in $c$ colors, where $1\le c\le b$, and let $n_{i}$ be the number of bowls of color $i$ where $0 \le i < c$.
It is possible to organize the bowls into $s$ nonempty stacks such that no stack contains a repeated color *if and only if*

$$
\max_{0\le i\le c-1} n_i\le s\le b.
$$

<div class="vspace"></div>

We'll leave the necessity proof as an exercise.

