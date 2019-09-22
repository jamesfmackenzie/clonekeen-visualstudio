# clonekeen (Visual C++)

A Visual Studio port of CloneKeen

CloneKeen is very nearly complete clone of one of the classic DOS game, Commander Keen: Invasion of the Vorticons by ID. The game is written in pure C, uses SDL, and is generally very easy to compile on most platforms. The original author is Caitlin Shaw. For more info see http://clonekeen.sourceforge.net/?nav=main

This version has been migrated from Makefile to a Visual Studio Project and now uses NuGet. Should be very easy to build and run with Visual Studio:

1. Checkout and open Solution file in Visual Studio 
2. Restore NuGet dependencies (Project -> Manage NuGet Dependencies > Restore)
3. Build (Build > Build Solution)
4. Run (Debug > Start Without Debugging) or Debug (Debug > Start Debugging)

Game data is automatically copied to the Target Directory as part of build - so everything should "just run"
