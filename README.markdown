CryMono (v0.7-dev) - CryENGINE3 game development on the .NET/Mono platform
	by Poppermost Productions. (Based on 'cemono' by Sam 'ins\' Neirinck)

# Description
CryMono brings the power of .NET/Mono into the world of CryENGINE3, allowing game logic to be scripted in a faster and easier to use lanaguage than Lua.
	
## Project Status
The project is no longer being maintained, due to the inability for previous contributors to devote time to the project. Unfortunately all our documentation went down a while back after a server mishap, so this repo is what remains, with what inline code comments there are.

At the moment I recommend keeping an eye on <a href="https://github.com/RoqueDeicide/CryCIL">CryCIL</a> by RoqueDeicide, he has picked up the project and is heavily rewriting it for what appears to be a tool meant for production in modern CRYENGINE.

Thanks for all that kept an eye on the project in the past, was really fun working on. Note that I still really emphasise how important what CryMono meant to do is, .NET <b>does</b> allow for rapid iteration, especially when combined with tools such as runtime reloading of scripts. Hopefully this'll continue in some form for CRYENGINE, trends are showing that this is something people are noticing: https://mono-ue.github.io/.
	
### Documentation & Info
To find out more about CryMono, visit our main page at <a href="http://crymono.inkdev.net">crymono.inkdev.net</a>!

#### Source directory structure 
Our Visual Studio projects have been set up to expect all contents to be placed inside a folder within the Code folder shipped with the CryENGINE Free SDK.

Example:
C:\CryENGINE\Code\CryMono

Using another folder structure is up to the developer, but will require customization in order for reference and output locations to be correct.
