---
layout: page
title: Stacking Bowls
---

While putting dishes away, a question occurred to me. I have 12 bowls in 6 colors (2 each). The cupboard height limits stacks of bowls to 4. So, there are three stacks. The question was, "Can I create stacks where no stack as a repeat color?"

It turns out in my case, yes. Let's formalize the above:

**Propositon** Suppose we have $b > 0$ bowls in $c > 0$, with $c \mid b$ and each color having $b/c$ bowls. It is possible to group them into $s$ stacks, $s > 0$, with $s \mid b$, each stack having $b/s$ bowls, such that no two stacks contain repeat colors.

**Proof**

First, observe that 

$$\dfrac{b}{c} \le s$$

since every copy of a color has to go on a different stack. Further, 

$$\dfrac{b}{s} \le c$$

Both expressions are equivalent to 

$$b \le cs$$

This given $b, s, c > 0$, if $b < cs$, we can create the stacks. $\qquad \square$


<div class="vspace"></div>

Notice that the proof did not rely on the fact that $c$ and $s$ divide $b$ nor did it require the stacks be the same size. This means we should be able to weaken the assumptions:

$$ c | b \text{ becomes } 1 <= c \le b$$

and 

$$ s | b \text{ becomes } 1 <= s \le b$$

The first should be clear: you can't have more colors than bowls. The second just makes the problem a little tidier. If $s > b$ then the problem is trivial there will be two types of stacks, stacks with exactly one bowl and stacks with no bowls. We simply want to avoid the "no bowl" stacks for this problem.

We will also require that no stack is empty. This removes trivial solutions.

<div class="vspace"></div>

**Theorem.** Suppose we have $b>0$ bowls in $c$ colors, where $1\le c\le b$, and let $n_{i-1}$ be the number of bowls of color $i$.  Suppose we want to organize the bowls into $s$ nonempty stacks such that no stack contains a repeated color. It is possilbe to create such stacks if

$$
\max_{0\le i\le c-1} n_i\le s\le b.
$$

**Proof:** 

Suppose

$$
\max_{0\le i\le c-1} n_i\le s\le b.
$$

We have two things to show: each stack is non-empty and no stack has a repeated color. Since we need to demonstrate we *can* create the stacks, we explicit describe the construction.

Start by listing the colors, $C_0, C_1, ... C_{c-1}$ and the stacks $S_0, S_1, ... S_{s-1}$. We will create the stacks in the following way: 

Start with color $C_0$. Place one bowl of color $C_0$ on consecutive stacks until you run out of bowls. Since there are $n_i$ bowls of color $C_i$ and $n_i \le s$, the first distribution will look like

$$
\begin{array}{c|cccc}
\text{color} & C_0 & C_0 & ... & C_0 \\
\text{stack} & S_0 & S_1 & ... & S_{n_0} \\
\end{array}
$$

We then begin distributing the bowls of color $C_1$ in the same way:

$$
\begin{array}{c|cccccccccccc}
\text{color} & C_0 & C_0 & ... & C_0     &  C_1                  & ... & C_1                  & ... & C_1 \\
\text{stack} & S_0 & S_1 & ... & S_{n_0} &  S_{(n_0 + 1)\mod{s}} & ... & S_{(n_0 + 2)\mod{s}} & ... & S_{(n_0 + n_1)\mod{s}}\\
\end{array}
$$

If $n_0 + n_1 \le s$, we have covered the first $n_0 + n_1$ stacks with one bowl and the remaining $s - (n_0 + n_1)$ stacks are empty. If $n_0 + n_1 > s$, then we would wrap back to $S_0$ and continue distributing. Since $n_i \le s$, we will never place two bowls of the same color on the same stack. This means that no stack will ever have two bowls of the same color.

This leaves us to show that no stack is non-empty. This is due to our construction. If $b > s$ and we are distributing the bowls one at a time in a cyclical way, every stack will have at least one bowl. $\qquad \square$


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
