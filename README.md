# LiteLoaderBDS C++ Plugin Template

The template repository for LiteLoaderBDS C++ plugin development.

## Usage

### Start to write your plugin

1. Click the `Use this template` button on the top right.

2. Fill up basic information and create your plugin repository.

3. Clone your plugin repository.

4. Fill up the information in **/src/Version.h** and remove the line with *static_assert*.

5. Write your code in **/src/Plugin.cpp**. You can also create more C++ files in **/src**.

### Build your plugin

#### Build with CMake

1. Create the directory **/build**.

2. Go into **/build**.

3. Run command `cmake .. && cmake --build . -j`

#### Build with Visual Studio

1. Open **/Plugin.sln**.

2. Click **Build->Build Solution** or press **Ctrl+Shift+B** to build.

### Deploy your plugin

1. Get your plugin in **/build/Release**.
    * **Plugin.dll** is your plugin. You can put it into the **plugin** directory of LiteLoaderBDS and it will be loaded automatically.
    * **Plugin.pdb** is the program database for debugging. Put it into the **plugin** directory of LiteLoaderBDS, and the time your plugin crashes accidentally, you can get a readable crash report in the **log** directory.

2. Run LiteLoaderBDS.

### Edit README.md and LICENSE

You ought not to keep the original README.md, for it contains instructions on how to build your own plugin, which might not fit your plugin repository.
The README.md of your repository should contain instructions on how to build, install and use your plugin.

You don't necessarily want your repository to be open sourced under the Unlicense, so please choose your own license in place of the `LICENSE` file.

## License

This repository is open source under the Unlicense.
Please refer to [the license file](LICENSE) for further information.