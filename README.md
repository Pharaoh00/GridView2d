# GridView

Viewport 2D estilo CAD feito com **Qt + OpenGL**, com grade infinita, pan/zoom, seleção e ferramentas de medição.  
Focado em uma base limpa para construir interfaces de visualização e interação em 2D (mapas, editores, plantas, etc.).

---

## ✨ Funcionalidades

- Grade infinita com linhas **minor/major** e adaptação ao zoom
- **Pan** e **Zoom** suaves (mundo ↔ tela)
- **Seleção** por clique (hit-test) e destaque visual
- Ferramenta de **medição** com régua/overlay e leitura em unidades do mundo
- Overlays de UI (status do mouse/zoom, âncora, modo atual)
- Pipeline OpenGL 3.3 Core com shaders simples e desenho eficiente de linhas

## 🎮 Controles

- **Arrastar com botão esquerdo:** pan
- **Roda do mouse:** zoom no cursor
- **Clique:** selecionar elemento
- **Modo Medição:** [descreva aqui se é tecla, botão, ou clique + arrasto]

## 🧱 Tecnologias

- C++
- Qt 6 (QOpenGLWidget / eventos / UI)
- OpenGL 3.3 Core
- CMake

## 🚀 Build

### Pré-requisitos

- Compilador C++ (MSVC, clang ou GCC)
- CMake 3.20+
- Qt 6

> Dica: no Windows, é comum usar Qt via **vcpkg**.