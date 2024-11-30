@echo off
cd /D E:\college\sem7\media\Idol-zombie-survival\out\build\x86-debug || (set FAIL_LINE=2& goto :ABORT)
"C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" -E copy_directory E:/college/sem7/media/Idol-zombie-survival/assets E:/college/sem7/media/Idol-zombie-survival/out/build/x86-debug/assets || (set FAIL_LINE=3& goto :ABORT)
cd /D E:\college\sem7\media\Idol-zombie-survival\out\build\x86-debug || (set FAIL_LINE=4& goto :ABORT)
"C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" -E copy E:/college/sem7/media/Idol-zombie-survival/libs/glew/bin/glew32.dll E:/college/sem7/media/Idol-zombie-survival/out/build/x86-debug || (set FAIL_LINE=5& goto :ABORT)
"C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" -E copy E:/college/sem7/media/Idol-zombie-survival/libs/glut/bin/glut32.dll E:/college/sem7/media/Idol-zombie-survival/out/build/x86-debug || (set FAIL_LINE=6& goto :ABORT)
cd /D E:\college\sem7\media\Idol-zombie-survival\out\build\x86-debug || (set FAIL_LINE=7& goto :ABORT)
"C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" -E copy E:/college/sem7/media/Idol-zombie-survival/libs/glew/bin/glew32.dll E:/college/sem7/media/Idol-zombie-survival/out/build/x86-debug || (set FAIL_LINE=8& goto :ABORT)
"C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" -E copy E:/college/sem7/media/Idol-zombie-survival/libs/glut/bin/glut32.dll E:/college/sem7/media/Idol-zombie-survival/out/build/x86-debug || (set FAIL_LINE=9& goto :ABORT)
"C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" -E copy E:/college/sem7/media/Idol-zombie-survival/libs/assimp/bin/assimp-vc143-mt.dll E:/college/sem7/media/Idol-zombie-survival/out/build/x86-debug || (set FAIL_LINE=10& goto :ABORT)
"C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" -E copy E:/college/sem7/media/Idol-zombie-survival/libs/assimp/bin/poly2tri.dll E:/college/sem7/media/Idol-zombie-survival/out/build/x86-debug || (set FAIL_LINE=11& goto :ABORT)
"C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" -E copy E:/college/sem7/media/Idol-zombie-survival/libs/assimp/bin/draco.dll E:/college/sem7/media/Idol-zombie-survival/out/build/x86-debug || (set FAIL_LINE=12& goto :ABORT)
"C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" -E copy E:/college/sem7/media/Idol-zombie-survival/libs/assimp/bin/minizip.dll E:/college/sem7/media/Idol-zombie-survival/out/build/x86-debug || (set FAIL_LINE=13& goto :ABORT)
"C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" -E copy E:/college/sem7/media/Idol-zombie-survival/libs/assimp/bin/pugixml.dll E:/college/sem7/media/Idol-zombie-survival/out/build/x86-debug || (set FAIL_LINE=14& goto :ABORT)
"C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" -E copy E:/college/sem7/media/Idol-zombie-survival/libs/assimp/bin/zlib1.dll E:/college/sem7/media/Idol-zombie-survival/out/build/x86-debug || (set FAIL_LINE=15& goto :ABORT)
cd /D E:\college\sem7\media\Idol-zombie-survival\out\build\x86-debug || (set FAIL_LINE=16& goto :ABORT)
"C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" -E copy E:/college/sem7/media/Idol-zombie-survival/libs/glew/bin/glew32.dll E:/college/sem7/media/Idol-zombie-survival/out/build/x86-debug || (set FAIL_LINE=17& goto :ABORT)
"C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" -E copy E:/college/sem7/media/Idol-zombie-survival/libs/glut/bin/glut32.dll E:/college/sem7/media/Idol-zombie-survival/out/build/x86-debug || (set FAIL_LINE=18& goto :ABORT)
"C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" -E copy E:/college/sem7/media/Idol-zombie-survival/libs/assimp/bin/assimp-vc143-mt.dll E:/college/sem7/media/Idol-zombie-survival/out/build/x86-debug || (set FAIL_LINE=19& goto :ABORT)
cd /D E:\college\sem7\media\Idol-zombie-survival\out\build\x86-debug || (set FAIL_LINE=20& goto :ABORT)
"C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" -E copy_directory E:/college/sem7/media/Idol-zombie-survival/libs E:/college/sem7/media/Idol-zombie-survival/out/build/x86-debug/libs || (set FAIL_LINE=21& goto :ABORT)
goto :EOF

:ABORT
set ERROR_CODE=%ERRORLEVEL%
echo Batch file failed at line %FAIL_LINE% with errorcode %ERRORLEVEL%
exit /b %ERROR_CODE%