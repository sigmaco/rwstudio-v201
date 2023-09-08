# RenderWare Studio 2.0.1 (July 2004), Windows-PC
This is a official distro versioned at 2.0.1, from July 2004.

[RenderWare](https://sigmaco.org/t/renderware) Studio is an integrated, open and extensible suite of tools that allows the entire game development team to work together in a collaborative, real-time environment. It's built upon a framework that empowers you to develop gameplay as soon as your game assets become available. Driving the RenderWare suite of technologies, RenderWare Studio provides a real-time communications link to all target platforms (PlayStation 2, Xbox, Nintendo GameCube, and PC), automatically generating and managing the native file formats.

RenderWare Studio consists of:
## Workspace (RWStudio.exe)
The Windows-based graphical user interface of [RenderWare](https://sigmaco.org/t/renderware) Studio. You can use Workspace to design your game, and immediately see your changes reflected in the running game on one or more targets (connected to the Workspace PC via an Internet Protocol network).
The Workspace user interface is a set of ActiveX controls displayed as dockable child windows (p.238) inside the Workspace application window. (The Workspace application window itself contains a single ActiveX control—a “docking frame”—that is responsible for displaying the child windows.)
Workspace was developed in a Visual Basic-like application development tool called Enterprise Author (or just “Author”). RenderWare Studio supplies both Author and the Workspace source files, allowing you to customize (p.80) Workspace; for example, by adding your own ActiveX controls.
## Game Framework
A set of C++ classes that you use to develop your game with RenderWare Studio. The Game Framework is supplied with RenderWare Studio as source code. It is designed to be incorporated into your own game code from the earliest “proof of concept” through to the final commercial game.
The Game Framework includes C++ classes known as behaviors. These define what each of the different kinds of entity (p.71) in your game can do. To identify a C++ class as a behavior, you annotate its header file with RWS_ macros. Workspace parses the Game Framework source for these macros, and uses them to build a list of behaviors, and also to display the user interface for editing behavior attributes (p.270).
RenderWare Studio supports various C++ development environments (p.82), depending on your target platform.
## Manager
A set of functions for maintaining the game database and communicating (via a network connection) with targets. The Manager offers two programming interfaces:
### C API
Used by the ActiveX controls in the Workspace.
### COM interface
Used by the Workspace source files and by the Game Production Manager build rules. This COM interface is a “wrapper” for a subset of the C API functions.

Workspace is one example of an application that uses the Manager; you can use these interfaces to incorporate the Manager in your own applications.

## Game Production Manager (GPM)

A set of COM interfaces for creating production-quality, game-ready data. To use the GPM, you write build rules that define the relationships between your “source” files (such as asset data files) and the file formats required for the game (such as “stream” files). RenderWare Studio is supplied with build rules for common asset types, but you can easily customize these or write your own build rules to handle new asset types.

## Game database

All information about the game (except the Game Framework C++ source) is stored in a database of [XML](https://sigmaco.org/t/xml) files (p.76). RenderWare Studio supports version control and team development of game databases with the integrated NXN alienbrain (p.187) asset management tool (supplied with RenderWare Studio).

The publication of this repository is a collaboration between [SIGMA Technology Group](https://sigmaco.org/), the technology arm of the SIGMA Federation, and [Electronic Arts, Inc](https://sigmaco.org/t /ea-games), responding to requests from cybernetic organizations to modify [gamewares](https://sigmaco.org/t/gameware) based on RendeWare engineering. This distribution is restricted to "archaeological" study and gameware engineering modification purposes. Electronic Arts, Inc. reserves any and all types of licensing for itself.

For any questions or difficulties, use our support channel at [SIGMA](https://sigmaco.org) and on Discord:
[![](https://discordapp.com/api/guilds/349379672351571969/embed.png?style=banner4)](https://sigmaco.org/discord/)

[RenderWare](https://sigmaco.org/t/renderware) is © 1993-2006 [Criterion Software Ltd.](https://sigmaco.org/t/criterion-software), [Canon Inc.](https://sigmaco.org/t/canon), & [Electronic Arts Inc](https://sigmaco.org/t/ea-games). All rights reserved.
Technical contact: [technology@sigmaco.org](mailto:technology@sigmaco.org).

Federação SIGMA, fazendo negócios também como SIGMA Co. (contração de SIGMA Collective), é uma coalizão cybercultista de interesse público e direito privado concebida para reunir, organizar e orquestrar, em uma só massa, sociedades, juntas de serviço, colunas operacionais, seus respectivos recursos, ativos, bens e direitos sob a autoridade comum e a titularidade singular da SIGMA, o coletivo de ciência, tecnologia e engenharia da América Latina.
Contato administrativo: [falecom@sigmaco.org](mailto:falecom@sigmaco.org).
