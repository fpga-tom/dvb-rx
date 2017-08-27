<project xmlns="com.autoesl.autopilot.project" name="rx" top="_ofdm_ifft">
    <includePaths/>
    <libraryPaths/>
    <Simulation>
        <SimFlow name="csim" optimizeCompile="true" csimMode="0" lastCsimMode="0" compiler="true"/>
    </Simulation>
    <files xmlns="">
        <file name="../src/tb.h" sc="0" tb="1" cflags=" "/>
        <file name="../src/tb.cpp" sc="0" tb="1" cflags=" -std=c++0x"/>
        <file name="../src/sync_update_tb.cpp" sc="0" tb="1" cflags=" -std=c++0x"/>
        <file name="../src/sync_find_peak_tb.cpp" sc="0" tb="1" cflags=" -std=c++0x"/>
        <file name="../src/sync_correlate_tb.cpp" sc="0" tb="1" cflags=" -std=c++0x"/>
        <file name="../src/ofdm_ifft_tb.cpp" sc="0" tb="1" cflags=" "/>
        <file name="rx/src/ofdm.h" sc="0" tb="false" cflags=""/>
        <file name="rx/src/ofdm.cpp" sc="0" tb="false" cflags=""/>
        <file name="rx/src/config.h" sc="0" tb="false" cflags=""/>
        <file name="rx/src/Sync.h" sc="0" tb="false" cflags=""/>
        <file name="rx/src/Sync.cpp" sc="0" tb="false" cflags=""/>
    </files>
    <solutions xmlns="">
        <solution name="solution1" status="active"/>
    </solutions>
</project>

