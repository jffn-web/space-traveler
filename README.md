# Space Traveler

Jogo inspirado em Space Invaders, feito em C usando Raylib. Você controla uma X-Wing e precisa destruir os inimigos antes que eles cheguem embaixo.

## Funcionalidades

- Jogador com três vidas e sprites que mudam conforme o dano.
- Inimigos organizados em ondas.
- Escudos destrutíveis, restaurados a cada nova onda.
- Death Star que concede pontos extras.
- Menu com opções de jogar, ranking e sair.
- Ranking com as cinco melhores pontuações, salvo em arquivo.

## Controles

| Tecla | Ação |
|---|---|
| A / D | Mover a nave |
| Espaço | Atirar |
| W / S | Selecionar opção no menu |
| Enter | Confirmar |
| Backspace | Apagar letra ao digitar o nome ou voltar do ranking |
| Esc | Fechar o jogo |

## Compilação

O projeto utiliza CMake 3.24 ou superior e Ninja.
A Raylib é baixada automaticamente durante a primeira configuração, que precisa de conexão com a internet.

Compilação e execução testadas no Windows com MSYS2 UCRT64.
As instruções para Linux e macOS ainda não foram testadas neste projeto.

### Windows — MSYS2 UCRT64

Instale o MSYS2: https://www.msys2.org/

Abra o terminal MSYS2 UCRT64 e instale as ferramentas:

```bash
pacman -S --needed git mingw-w64-ucrt-x86_64-gcc mingw-w64-ucrt-x86_64-cmake mingw-w64-ucrt-x86_64-ninja
```

### Linux — Ubuntu/Debian

Instale as ferramentas e dependências:

```bash
sudo apt update
sudo apt install build-essential git cmake ninja-build libasound2-dev libx11-dev libxrandr-dev libxi-dev libgl1-mesa-dev libglu1-mesa-dev libxcursor-dev libxinerama-dev libwayland-dev libxkbcommon-dev
```

Confira se o CMake instalado atende à versão mínima:

```bash
cmake --version
```

### macOS

Instale as ferramentas de desenvolvimento:

```bash
xcode-select --install
```

Após concluir a instalação, instale o Homebrew caso ainda não tenha:
https://brew.sh/

Com o Homebrew disponível:

```bash
brew install cmake ninja
```

### Baixar e compilar o projeto

Após preparar as ferramentas do seu sistema:

```bash
git clone https://github.com/jffn-web/space-traveler.git
cd space-traveler
cmake -S . -B build-cmake -G Ninja -DCMAKE_BUILD_TYPE=Release
cmake --build build-cmake
```

### Executar

Execute os comandos a partir da pasta principal do projeto, onde estão `assets` e `CMakeLists.txt`.

No Windows, pelo MSYS2 UCRT64:

```bash
./build-cmake/space-traveler.exe
```

No Linux ou macOS:

```bash
./build-cmake/space-traveler
```

Cada sistema precisa compilar seu próprio executável.

## Ranking

O arquivo `ranking.txt` é criado automaticamente ao salvar a primeira pontuação, caso ainda não exista.

Ele guarda até cinco resultados, em ordem da maior pontuação para a menor. Seguindo os comandos acima, fica na pasta principal do projeto.

## Vídeo

[Assista ao vídeo do jogo](https://youtu.be/ajhaXkjtVCE)