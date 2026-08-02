@echo off
@echo Configure PLT for MinGW.
rm -f Makefile tplt/Makefile out/Makefile pltfnt/Makefile tek/Makefile
cp -f makefile.mgw Makefile
cp -f tplt/makefile.mgw tplt/Makefile
cp -f out/makefile.mgw out/Makefile
cp -f pltfnt/makefile.mgw pltfnt/Makefile
cp -f tek/makefile.mgw tek/Makefile
@echo "Type 'make' then 'make install'."
