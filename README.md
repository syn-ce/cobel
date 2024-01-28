# Cobel

Sobel edge detection for the PNG file format written in C.

## Parameters

`cobel [FILENAME_IN] [COLOR] [FILENAME_OUT] [NR_ITERATIONS] [...S_X] [...S_Y]`

where

-   `FILENAME_IN` specifies the filename of the png to read. Required.
-   `COLOR` specifies whether to keep the color-channels separate (i.e. colorful, 1) or combine them (i.e. black and white, 0). Optional. Defaults to $0$ (black and white).
-   `FILENAME_OUT` specifies the filename to save the resulting png to. Optional. Defaults to $\text{"output.png"}$.
-   `NR_ITERATIONS` specifies the number of times the (sobel-) operator should be applied (e.g. 2 times would be the same as applying it again to the output file of the first time). Optional. Defaults to $1$
-   `[...S_X]` and `[...S_Y]` (optional) specify the operator in the $x$- and $y$-direction respectively, the values of which are being parsed in order $x_{00}$, $x_{01}$, $x_{02}$, $x_{10}$, ..., $x_{22}$, $y_{00}$, $y_{01}$, ..., $y_{22}$. Optional. Default to $S_X=[[-1,0,1], [-2,0,2], [-1,0,1]]$ and $S_Y=[[-1,-2,-1], [0,0,0], [1,2,1]]$ (the [Sobel operator](https://en.wikipedia.org/wiki/Sobel_operator)).

## Examples

| Original                                                                                                                                                                                                                                                                                               |
| ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------ |
| <p align="center"> <img alt="green Julia Set with c-value of around x=1.145, y=0.209 with low exponent" src="/imgs/Original/JuliaSet_112_234_94_320_1.1544476768316247_1665_0.20911330049261068_0.1921182266009851_-1.018685076478028_0.7278396510857144_2.403432664568266_LC_0.jpg" width="60%"> </p> |

| 1 Iteration                                                                                              | 2 Iterations                                                                                              |
| -------------------------------------------------------------------------------------------------------- | --------------------------------------------------------------------------------------------------------- |
| ![sobel edge detection applied once to the image of the Julia Set](/imgs/BlackWhite/JuliaSet_Sobel1.jpg) | ![sobel edge detection applied twice to the image of the Julia Set](/imgs/BlackWhite/JuliaSet_Sobel2.jpg) |

| 3 Iterations                                                                                                    | 4 Iterations                                                                                                   |
| --------------------------------------------------------------------------------------------------------------- | -------------------------------------------------------------------------------------------------------------- |
| ![sobel edge detection applied three times to the image of the Julia Set](/imgs/BlackWhite/JuliaSet_Sobel3.jpg) | ![sobel edge detection applied four times to the image of the Julia Set](/imgs/BlackWhite/JuliaSet_Sobel4.jpg) |

### With color

| 1 Iteration                                                                                                         | 2 Iterations                                                                                                         |
| ------------------------------------------------------------------------------------------------------------------- | -------------------------------------------------------------------------------------------------------------------- |
| ![sobel edge detection with color applied once to the image of the Julia Set](/imgs/Color/JuliaSet_Sobel1Color.jpg) | ![sobel edge detection with color applied twice to the image of the Julia Set](/imgs/Color/JuliaSet_Sobel2Color.jpg) |

| 3 Iterations                                                                                                               | 4 Iterations                                                                                                              |
| -------------------------------------------------------------------------------------------------------------------------- | ------------------------------------------------------------------------------------------------------------------------- |
| ![sobel edge detection with color applied three times to the image of the Julia Set](/imgs/Color/JuliaSet_Sobel3Color.jpg) | ![sobel edge detection with color applied four times to the image of the Julia Set](/imgs/Color/JuliaSet_Sobel4Color.jpg) |

## Changing the Operator

| [-2 0 -2] as first row of S_X                                                                                                                                                          | [-2 1 -2] as first row of S_X                                                                                                                                                     |
| -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| ![sobel edge detection with with first row of S_X equal to [-2, 0, -2] applied three times to the image of the Julia Set](/imgs/OtherOperator/JuliaSet_SobelOtherOperator_-2_0_-2.jpg) | ![sobel edge detection with first row of S_X equal to [-2, 0, -2] applied three times to the image of the Julia Set](/imgs/OtherOperator/JuliaSet_SobelOtherOperator_-2_1_-2.jpg) |
