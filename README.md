# Cobel

Sobel edge detection for the PNG file format written in C.

## Parameters

`cobel [FILENAME_IN] [COLOR] [FILENAME_OUT] [NR_ITERATIONS] [...S_X] [...S_Y]`

where

-   `FILENAME_IN` specifies the filename of the png to read. Required.
-   `COLOR` specifies whether to keep the color-channels separate (i.e. colorful, 1) or combine them (i.e. black and white, 0). Optional. Defaults to $0$ (black and white).
-   `FILENAME_OUT` specifies the filename to save the resulting png to. Optional. Defaults to $\text{"output.png"}$.
-   `NR_ITERATIONS` specifies the number of times the (sobel-) operator should be applied (e.g. 2 times would be the same as applying it again to the output file of the first time). Optional. Defaults to $1$
-   `[...S_X]` and `[...S_Y]` (optional) specify the operator in the $x$- and $y$-direction respectively, the values of which are being parsed in order $x_{00}$, $x_{01}$, $x_{02}$, $x_{10}$, ..., $x_{22}$, $y_{00}$, $y_{01}$, ..., $y_{22}$. Optional. Default to $S_X=[[-1,0,1], [-2,0,2], [-1,0,1]]$ and $S_Y=[[-1,-2,-1], [0,0,0], [1,2,1]]$.

## Examples

| Original                                                                                                                                                                              |
| ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| <img src="/imgs/JuliaSet_112_234_94_320_1.1544476768316247_1665_0.20911330049261068_0.1921182266009851_-1.018685076478028_0.7278396510857144_2.403432664568266_LC_0.jpg" width="60%"> |

| 1 Iteration                    | 2 Iterations                   |
| ------------------------------ | ------------------------------ |
| ![](/imgs/JuliaSet_Sobel1.jpg) | ![](/imgs/JuliaSet_Sobel2.jpg) |

| 3 Iterations                   | 4 Iterations                   |
| ------------------------------ | ------------------------------ |
| ![](/imgs/JuliaSet_Sobel3.jpg) | ![](/imgs/JuliaSet_Sobel4.jpg) |

### With color

| 1 Iteration                         | 2 Iterations                        |
| ----------------------------------- | ----------------------------------- |
| ![](/imgs/JuliaSet_Sobel1Color.jpg) | ![](/imgs/JuliaSet_Sobel2Color.jpg) |

| 3 Iterations                        | 4 Iterations                        |
| ----------------------------------- | ----------------------------------- |
| ![](/imgs/JuliaSet_Sobel3Color.jpg) | ![](/imgs/JuliaSet_Sobel4Color.jpg) |

## Changing the Operator

| [-2 0 -2] as first row of operator                 | [-2 1 -2] as first row of operator                 |
| -------------------------------------------------- | -------------------------------------------------- |
| ![](/imgs/JuliaSet_SobelOtherOperator_-2_0_-2.jpg) | ![](/imgs/JuliaSet_SobelOtherOperator_-2_1_-2.jpg) |
