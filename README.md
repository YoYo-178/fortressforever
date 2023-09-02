# [Fortress Forever](http://www.fortress-forever.com)

A Team Fortress mod on the Source Engine (Source SDK 2006)

### <ins>**WARNING**</ins>
**USE THIS SETUP AT YOUR OWN RISK.**
THE LIBRARIES PRODUCED BY THIS SETUP **COULD BE UNSTABLE**.

### Compiling

Fortress Forever can now be compiled using Visual Studio 2022. The following instructions show how to set up a Visual Studio 2022 development environment for Fortress Forever.

1. **Install Visual Studio 2022**
  * Download and install Visual Studio 2022 Community Edition [from here](https://visualstudio.microsoft.com/thank-you-downloading-visual-studio/?sku=Community&channel=Release&version=VS2022).
2. **Install the Microsoft Platform SDK**
  * Download and install the [Windows Server 2003 SP1 Platform SDK](https://www.microsoft.com/en-us/download/details.aspx?id=15656)
    * You only need to install *Microsoft Windows Core SDK*
3. **Install Windows SDK**
  * Download and install the Windows SDK [Installer](https://go.microsoft.com/fwlink/?linkid=2237387) or [from .ISO](https://go.microsoft.com/fwlink/?linkid=2237510)
4. **Download the VS 2005 includes and libraries**
  * Download the [.rar file](https://drive.google.com/file/d/1HPKgTVpzB5pSSQhMJ_J8i2uP-JgO9jyA/view?usp=sharing) and extract its contents to `C:\Program Files\Microsoft Visual Studio\2022\Community\VC`.
5. **Build Fortress Forever**
  * Open `Game_Scratch-2005.sln` and run *Build Solution*
  * **Note:** The compiled .dlls will automatically get copied to `<SteamDirectory>\SteamApps\common\Fortress Forever\FortressForever\bin`

### Debugging
To start the game from within VS debugger, right click `client_ff` or `server_ff` project (whichever you are working on)
and click `properties`. Navigate to the 'Debugging' section.
Set command to the 'hl2.exe' in your fortress forever installation directory. On a default steam installation it will look like this:

- command `C:\Program Files (x86)\Steam\steamapps\common\Fortress Forever\hl2.exe`
- arguments: `-game "FortressForever" -allowdebug -dev`
- working directory: `C:\Program Files (x86)\Steam\steamapps\common\Fortress Forever\`

 (you will need to set this up for both client/server, or whatever you need to debug)

Now you can smack F5 to start the game with debugging ready to go. I recommend adding eg, `+map ff_2fort` to parameters save time.
Here is an example with nonstandard steam path: ![example](https://i.imgur.com/98WRQDI.png) 

#### Debugging an active session
* Compile using the *Debug FF* configuration
* Launch Fortress Forever (need to use the launch parameter `-allowdebug` in steam)
* In Visual Studio, go to *Debug* -> *Attach to Process*
* Find `hl2.exe` in the list and click *Attach*

#### Debugging a crash log
* Open the crash log (.dmp/.mdmp) in Visual Studio 2022
* Go to *Debug* -> *Start Debugging*
  * Check the output window to see if symbols were successfully loaded for `fortressforever\bin\server.dll` and `fortressforever\bin\client.dll`. If not, you'll probably need to copy the dump file to a directory containing the correct .dll and .pdb files for the version of the game that the crash occurred on.
  * If the crash points to *Disassembly*, the crash log will likely not be of much use, as that usually means that the crash occurred somewhere in the Source engine code that we don't have access to. However, it's a good idea to check the *Call Stack* window to see if the crash originated in FF code.
* **Note:** Crash logs can be found in the `<SteamDirectory>/dumps` directory, and will be named something like `assert_hl2.exe_<datetime>_1.dmp`
