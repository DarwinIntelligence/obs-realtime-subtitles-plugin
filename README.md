# OBS RealTime Subtitles Plugin #

This plugin is for use with Darwin Intelligence's RealTime subtitles AI product.

Currently in development so some bugs may occur. Requires Internet Access at all times of necessary Transcript/Translation

It is supported on Windows/Linux.

## Requirements

In order to use you will need an API Key which you can sign up for here by emailing - darwinai@darwinai.ai

## Features

- It currently supports the following base languages:
    - Korean (한국어)
    - Chinese (중국어)
    - Japanese (일분어)

## Installation (Windows)
To install this plugin, you will need the openssl library. To do so, you will use the windows installation tool Chocolatey. Installation steps are shown below. You can also find them by visiting the Chocolatey website (https://chocolatey.org/install).
1. Open a powershell terminal on your windows PC in Administrator mode. ![image](https://github.com/user-attachments/assets/428ba5ab-3052-4d8d-8ad4-b65c47e5f329)
2. Paste the following command into your terminal and press enter.
   Set-ExecutionPolicy AllSigned or Set-ExecutionPolicy Bypass -Scope Process
3. Now, to install chocolatey, paste the following command into your terminal.
   Set-ExecutionPolicy Bypass -Scope Process -Force; [System.Net.ServicePointManager]::SecurityProtocol = [System.Net.ServicePointManager]::SecurityProtocol -bor 3072; iex ((New-Object System.Net.WebClient).DownloadString('https://community.chocolatey.org/install.ps1'))
4. Now that Chocolatey is installed, you can paste the following command into your terminal. This installs openssl.
   choco install openssl
   a. Note, if the choco command doesn't work, you may have to close your terminal and repeat step 1.
5. Next you must copy Darwin's translator plugin into your obs plugin folder.
   a. Copy the two files recieved from Darwin. ![image](https://github.com/user-attachments/assets/76bacb94-ff44-4d5e-8e57-0018ffd1f04d)
   b. Paste them into your obs plugins folder. Usually this is located at "C:\Program Files\obs-studio\obs-plugins\64bit" ![image](https://github.com/user-attachments/assets/d13be809-dfbb-4def-8abe-5633e2e7fe2a)
6. Darwin Translation is now ready to use!




## Usage

1. Add a source for each microphone source (BJ) ![BJ1 Audio Source](./assets/1.png)
1. Slect Create new and name accordingly to person who will use mic ![Audio Source](./assets/2.png)
1. Select Source Properties ![Select Audio Source Properties](./assets/3.png)
1. Select Audio Device and close ![Audio Source Properties](./assets/4.png)
1. Select Filters ![BJ1 Audio Source](./assets/5.png)
1. Click + ![BJ1 Audio Source](./assets/6.png)
1. Select Darwin Translation ![BJ1 Audio Source](./assets/7.png)
1. Set filter name accordingly ![BJ1 Audio Source](./assets/8.png)
1. Select languages, if you want the transcript and translation to appear select both boxes or one or the other ![BJ1 Audio Source](./assets/9.png)
1. Select Text Source ![BJ1 Audio Source](./assets/10.png)
1. Select Font ![BJ1 Audio Source](./assets/11.png)
1. Select Writing system of output language ![BJ1 Audio Source](./assets/12.png)
1. Pick any font you prefer ![BJ1 Audio Source](./assets/13.png)
1. If speech has occured text will appear ![BJ1 Audio Source](./assets/14.png)
1. Enter properties of text source ![BJ1 Audio Source](./assets/15.png)
1. Set log lines and height and width of box ![BJ1 Audio Source](./assets/16.png)
1. Example Layout for Korean Text ![Example Layout for Korean Text](./assets/ExampleLayout.png)

## Troubleshooting
Here are some common issues from OBS and how to fix them for the plugin.
### When OBS opens, the subtitles are still there, but they aren't translating.
To fix this, click on the audio source -> filters -> and then toggle the translate or transcribe (whichever you want) off and on again. That will begin translation again.
### Translation doesn't start right away
This is normal. When a filter is started, it takes a few seconds to begin translating.
### Any other problem
If any other problem appears, please email Darwin AI. A common fix is deleting all of the sources with filters on it, exiting OBS, and then readding the sources. This is a project still under development so while the version you are using should be stable, there are still changes being made that could cause small issues. Once the translator is running, there should be no issues.
