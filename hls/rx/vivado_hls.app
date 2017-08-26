<project xmlns="com.autoesl.autopilot.project" name="rx" top="sync_update">
    <files>
        <file name="rx/src/IFFT.cpp" sc="0" tb="false" cflags=""/>
        <file name="rx/src/IFFT.h" sc="0" tb="false" cflags=""/>
        <file name="rx/src/Sync.cpp" sc="0" tb="false" cflags=""/>
        <file name="rx/src/Sync.h" sc="0" tb="false" cflags=""/>
        <file name="rx/src/config.h" sc="0" tb="false" cflags=""/>
        <file name="../src/sync_correlate_tb.cpp" sc="0" tb="1" cflags=" -std=c++0x"/>
        <file name="../src/sync_find_peak_tb.cpp" sc="0" tb="1" cflags=" -std=c++0x"/>
        <file name="../src/sync_update_tb.cpp" sc="0" tb="1" cflags="-std=c++0x"/>
        <file name="../src/tb.cpp" sc="0" tb="1" cflags="-std=c++0x"/>
    </files>
    <includePaths/>
    <libraryPaths/>
    <Simulation>
        <SimFlow name="csim" optimizeCompile="true" csimMode="0" lastCsimMode="0" compiler="true"/>
    </Simulation>
    <solutions xmlns="">
        <solution name="solution1" status="active"/>
    </solutions>
</project>

