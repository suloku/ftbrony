set ProjectName=ftbrony
echo ftbrony
res/arm-none-eabi-strip.exe ftbrony.elf
res/bannertool.exe makebanner -i res/banner.png -a res/audio.wav -o res/banner.bnr
res/bannertool.exe makesmdh -s "ftbrony" -l "ftbrony" -p "mtheall & iamevn" -i ftbrony.png  -o res/icon.icn
res/makerom.exe -f cci -rsf res/gw_workaround.rsf -target d -exefslogo -elf ftbrony.elf -icon res/icon.icn -banner res/banner.bnr -o ftbrony.3ds
res/makerom.exe -f cia -o ftbrony.cia -elf ftbrony.elf -rsf res/build_cia.rsf -icon res/icon.icn -banner res/banner.bnr -exefslogo -target t
echo Done!