#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <math.h>

#define PLANCK_CONSTANT 6.62607015e-34
#define BOLTZMANN_CONSTANT 1.380649e-23
#define SPEED_OF_LIGHT 299792458.0
#define WIEN 2.897771955e-3

#define CIRC_HEIGHT 17
#define CIRC_WIDTH 75
#define CIRC_ASPECT_RATIO 2
#define CIRC_RADIUS 5
#define CIRC_CENTER_X (CIRC_WIDTH / 2.0) 
#define CIRC_CENTER_Y (CIRC_HEIGHT / 2.0)
typedef struct {
  unsigned char red;
  unsigned char green;
  unsigned char blue;
}
ColorRGB;

int clip(int v, int max) {
  return v < max ? v : max - 1;
}

void printColor(ColorRGB color) {
  printf("RGB(%d, %d, %d)\n", color.red, color.green, color.blue);
}

void clearScreen() {
  printf("\033[H\033[J");
}

void setUnbufferedInput() {
  struct termios term;
  tcgetattr(0, & term);
  term.c_lflag &= ~ICANON;
  term.c_cc[VMIN] = 1;
  term.c_cc[VTIME] = 0;
  tcsetattr(0, TCSANOW, & term);
}

char getch() {
  char buf = 0;
  struct termios old = {0};
  if (tcgetattr(0, & old) < 0)
    perror("tcsetattr()");
  old.c_lflag &= ~ICANON;
  old.c_lflag &= ~ECHO;
  old.c_cc[VMIN] = 1;
  old.c_cc[VTIME] = 0;
  if (tcsetattr(0, TCSANOW, & old) < 0)
    perror("tcsetattr ICANON");
  if (read(0, & buf, 1) < 0)
    perror("read()");
  old.c_lflag |= ICANON;
  old.c_lflag |= ECHO;
  if (tcsetattr(0, TCSADRAIN, & old) < 0)
    perror("tcsetattr ~ICANON");
  return buf;
}
void spectral_color(double * r, double * g, double * b, double l) {
  double t;
  * r = 0.0;
  * g = 0.0;
  * b = 0.0;

  if ((l >= 400.0) && (l < 410.0)) {
    t = (l - 400.0) / (410.0 - 400.0);* r = +(0.33 * t) - (0.20 * t * t);
  } else if ((l >= 410.0) && (l < 475.0)) {
    t = (l - 410.0) / (475.0 - 410.0);* r = 0.14 - (0.13 * t * t);
  } else if ((l >= 545.0) && (l < 595.0)) {
    t = (l - 545.0) / (595.0 - 545.0);* r = +(1.98 * t) - (t * t);
  } else if ((l >= 595.0) && (l < 650.0)) {
    t = (l - 595.0) / (650.0 - 595.0);* r = 0.98 + (0.06 * t) - (0.40 * t * t);
  } else if ((l >= 650.0) && (l < 700.0)) {
    t = (l - 650.0) / (700.0 - 650.0);* r = 0.65 - (0.84 * t) + (0.20 * t * t);
  }

  if ((l >= 415.0) && (l < 475.0)) {
    t = (l - 415.0) / (475.0 - 415.0);* g = +(0.80 * t * t);
  } else if ((l >= 475.0) && (l < 590.0)) {
    t = (l - 475.0) / (590.0 - 475.0);* g = 0.8 + (0.76 * t) - (0.80 * t * t);
  } else if ((l >= 585.0) && (l < 639.0)) {
    t = (l - 585.0) / (639.0 - 585.0);* g = 0.84 - (0.84 * t);
  }

  if ((l >= 400.0) && (l < 475.0)) {
    t = (l - 400.0) / (475.0 - 400.0);* b = +(2.20 * t) - (1.50 * t * t);
  } else if ((l >= 475.0) && (l < 560.0)) {
    t = (l - 475.0) / (560.0 - 475.0);* b = 0.7 - (t) + (0.30 * t * t);
  }
}

ColorRGB kelvin_table[] = {
          {255, 56,  0    }, {255, 71,  0    }, {255, 83,  0    }, {255, 93,  0    }, {255, 101, 0    }, {255, 109, 0    },
          {255, 115, 0    }, {255, 121, 0    }, {255, 126, 0    }, {255, 131, 0    }, {255, 138, 18   }, {255, 142, 33   },
          {255, 147, 44   }, {255, 152, 54   }, {255, 157, 63   }, {255, 161, 72   }, {255, 165, 79   }, {255, 169, 87   },
          {255, 173, 94   }, {255, 177, 101  }, {255, 180, 107  }, {255, 184, 114  }, {255, 187, 120  }, {255, 190, 126  },
          {255, 193, 132  }, {255, 196, 137  }, {255, 199, 143  }, {255, 201, 148  }, {255, 204, 153  }, {255, 206, 159  },
          {255, 209, 163  }, {255, 211, 168  }, {255, 213, 173  }, {255, 215, 177  }, {255, 217, 182  }, {255, 219, 186  },
          {255, 221, 190  }, {255, 223, 194  }, {254, 225, 198  }, {253, 227, 202  }, {252, 228, 206  }, {251, 230, 210  },
          {250, 232, 213  }, {249, 233, 217  }, {248, 235, 220  }, {247, 236, 224  }, {246, 238, 227  }, {245, 239, 230  },
          {244, 240, 233  }, {243, 242, 236  }, {242, 243, 239  }, {241, 244, 242  }, {240, 245, 245  }, {239, 246, 247  },
          {238, 248, 251  }, {237, 249, 253  }, {235, 249, 255  }, {234, 247, 255  }, {233, 232, 255  }, {232, 245, 255  },
          {231, 243, 255  }, {230, 242, 255  }, {229, 241, 255  }, {228, 240, 255  }, {227, 239, 255  }, {225, 238, 255  },
          {223, 237, 255  }, {221, 236, 255  }, {219, 235, 255  }, {218, 234, 255  }, {217, 233, 255  }, {215, 232, 255  },
          {214, 231, 255  }, {210, 230, 255  }, {207, 230, 255  }, {205, 229, 255  }, {201, 229, 255  }, {198, 227, 255  },
          {196, 227, 255  }, {194, 226, 255  }, {192, 225, 255  }, {189, 225, 255  }, {186, 224, 255  }, {184, 223, 255  },
          {180, 223, 255  }, {178, 222, 255  }, {175, 221, 255  }, {171, 221, 255  }, {167, 220, 255  }, {164, 220, 255  },
          {161, 218, 255  }, {158, 218, 255  }, {157, 217, 255  }, {154, 217, 255  }, {150, 216, 255  }, {147, 216, 255  },
          {145, 215, 255  }, {141, 215, 255  }, {138, 214, 255  }, {135, 214, 255  }, {133, 213, 255  }, {130, 213, 255  },
          {128, 212, 255  }, {125, 212, 255  }, {123, 212, 255  }, {120, 211, 255  }, {118, 211, 255  }, {116, 210, 255  },
          {112, 210, 255  }, {107, 210, 255  }, {103, 209, 255  }
};
double intensidadeRadiacao(double temperatura, double comprimentoOnda) {
  double expoente = (PLANCK_CONSTANT * SPEED_OF_LIGHT) / (BOLTZMANN_CONSTANT * temperatura * comprimentoOnda);
  return (2 * PLANCK_CONSTANT * pow(SPEED_OF_LIGHT, 2)) / (pow(comprimentoOnda, 5) * (exp(expoente) - 1));
}

ColorRGB temperatureToColor(int temp) {
  ColorRGB color;

  // A verificação no array kelvin_table deve ser feita manualmente
  if (temp >= 1000 && temp <= 12000) {
    int index = (temp - 1000) / 100;
    color = kelvin_table[index];
  } else {
    color.red = 255;
    color.green = 255;
    color.blue = 255;
  }

  return color;
}
double wien(double temperatura) {
  double b = 2.897771955e-3; // Constante de Wien
  return b / temperatura;
}
int main() {
  int temp = 1000; // Temperatura inicial em Kelvin
  double temperatura = 100.0; // Temperatura inicial em Kelvin
  double numeroDouble;

  // Convertendo o inteiro para double
  int maxX = CIRC_WIDTH;
  int maxY = CIRC_HEIGHT;
  double comprimentoInicial = 4e-9;
  double comprimentoFinal = 7e-6;
  double passo = 1e-9;

  int alturaTela = 20; // Altura máxima da tela
  int larguraTela = 70; // Largura máxima da tela

  struct termios old_terminal; // Variável para armazenar configurações anteriores do terminal

  int totalColors = 256; // Número de cores a serem exibidas
  int colorsPerColumn = 32; //Número de cores a serem exibidas em cada coluna

  ColorRGB colors[totalColors];

  for (int i = 0; i < totalColors; i++) {
    double wavelength = 400.0 + i * (700.0 - 400.0) / totalColors;

    double r, g, b;
    spectral_color( & r, & g, & b, wavelength);

    colors[i].red = (int)(r * 255);
    colors[i].green = (int)(g * 255);
    colors[i].blue = (int)(b * 255);
  }

  int numLines = totalColors / colorsPerColumn;

  while (1) {
    setUnbufferedInput();
    clearScreen();
    ColorRGB color = temperatureToColor(temp);
    double maxIntensidade = 0.0;
    for (double lambda = comprimentoInicial; lambda <= comprimentoFinal; lambda += passo) {
      double intensidade = intensidadeRadiacao(temperatura, lambda);
      if (intensidade > maxIntensidade) {
        maxIntensidade = intensidade;
      }
    }

    char tela[alturaTela][larguraTela];

    // Preenche a tela com espaços em branco
    for (int i = 0; i < alturaTela; i++) {
      for (int j = 0; j < larguraTela; j++) {
        tela[i][j] = ' ';
      }
    }

    // Preenche os pontos da curva com '*'
    for (double lambda = comprimentoInicial; lambda <= comprimentoFinal; lambda += passo) {
      double intensidade = intensidadeRadiacao(temperatura, lambda);
      int altura = (int)(intensidade / maxIntensidade * (alturaTela - 1));
      int largura = (int)((lambda - comprimentoInicial) / (comprimentoFinal - comprimentoInicial) * (larguraTela - 1));

      tela[alturaTela - 1 - altura][largura] = '*';
    }
    // Exibe a tela
    for (int i = 0; i < alturaTela; i++) {
      for (int j = 0; j < larguraTela; j++) {}
      printf("\n");
    }

    // Exibe a tela com a curva e as cores espectrais
    for (int i = 0; i < alturaTela; i++) {
      for (int j = 0; j < larguraTela; j++) {
        if (tela[i][j] == '*') {
          int index = (int)((j / (double)(larguraTela - 1)) * totalColors);
          printf("\033[38;2;%d;%d;%dm*\033[0m", colors[index].red, colors[index].green, colors[index].blue);
        } else {
          printf("%c", tela[i][j]);
        }
      }
      printf("\n");
    }
    for (int y = 0; y < CIRC_HEIGHT; y++) {
      for (int x = 0; x < CIRC_WIDTH; x++) {
        float distance1 = sqrt(pow((x - (CIRC_CENTER_X - 27)) / CIRC_ASPECT_RATIO, 2) + pow(y - CIRC_CENTER_Y + 2, 2));
        float distance2 = sqrt(pow((x - (CIRC_CENTER_X - 18)) / CIRC_ASPECT_RATIO, 2) + pow(y - CIRC_CENTER_Y + 2, 2));
        float distance3 = sqrt(pow((x - CIRC_CENTER_X + 22) / CIRC_ASPECT_RATIO, 2) + pow(y - (CIRC_CENTER_Y + 2), 2));
        float distance4 = sqrt(pow((x - CIRC_CENTER_X - 20) / CIRC_ASPECT_RATIO, 2) + pow(y - CIRC_CENTER_Y, 2));

        int printX = CIRC_CENTER_X + x;
        int printY = CIRC_CENTER_Y + y;

        printX = clip(printX, maxX);
        printY = clip(printY, maxY);

        int r = distance1 <= CIRC_RADIUS ? color.red : 0;
        int g = distance2 <= CIRC_RADIUS ? color.green : 0;
        int b = distance3 <= CIRC_RADIUS ? color.blue : 0;

        // Impressão do caractere colorido
        if (distance4 <= CIRC_RADIUS) {
          printf("\033[38;2;%d;%d;%dmo", color.red, color.green, color.blue); // Combined circle
        } else {
          printf("\033[38;2;%d;%d;%dmo", r, g, b); // Original circles
        }
      }
      printf("\033[0m\n");
    }
    printf("\033[38;2;%d;%d;%dm###\n", color.red, color.green, color.blue);
    printf("\033[0m");
    printf("\nControles:\n");
    printf("'+' para aumentar a temperatura, '-' para diminuir a temperatura\n");
    printf("'q' para sair\n");

    printColor(color);
    numeroDouble = (double)temp;
    double expoente = (PLANCK_CONSTANT * SPEED_OF_LIGHT) / ((WIEN /numeroDouble  ) * BOLTZMANN_CONSTANT * numeroDouble);
    double exponencial = exp(expoente);
    double densidade_espectral = (2 * PLANCK_CONSTANT * SPEED_OF_LIGHT * SPEED_OF_LIGHT) / (pow((WIEN / numeroDouble), 5) * (exponencial - 1));

    printf("Temperatura: %.2f K\nComprimento de onda: %.2e m\nIntensidade de radiação espectral %.2e watts/m^2\n", numeroDouble, (WIEN /numeroDouble), densidade_espectral);

    char input = getch();
    if (input == 'q') {
      break;
    } else if ((input == '+' || input == 'd') && temp < 12000) {
      temp += 100;
      if (temperatura < 12000.0) {
        temperatura += 100.0;
      }
    } else if ((input == '-' || input == 'a') && temp > 1000) {
      temp -= 100;
      if (temperatura > 150.0) {
        temperatura -= 100.0;
      }
    }
  }
  struct termios term;
  tcgetattr(0, & term);
  term.c_lflag |= ICANON;
  tcsetattr(0, TCSANOW, & term);

  return 0;
}
