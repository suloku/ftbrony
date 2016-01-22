set ProjectName=ftbrony
echo ftbrony
res/arm-none-eabi-strip.exe ftbrony.elf
res/bannertool.exe makebanner -i res/banner.png -a res/audio.wav -o res/banner.bnr
res/bannertool.exe makesmdh -s "ftbrony" -l "ftbrony" -p "mtheall & iamevn" -i ftbrony.png  -o res/icon.icn
res/makerom.exe -f cia -o ftbrony.cia -DAPP_ENCRYPTED=false -rsf res/build_cia.rsf -target t -exefslogo -elf ftbrony.elf -icon res/icon.icn -banner res/banner.bnr -DAPP_SYSTEM_MODE="64MB" -DAPP_SYSTEM_MODE_EXT="Legacy"
echo Done!