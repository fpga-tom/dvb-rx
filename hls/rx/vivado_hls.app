<project xmlns="com.autoesl.autopilot.project" top="sync_update" name="rx">
    <includePaths/>
    <libraryPaths/>
    <Simulation>
        <SimFlow name="csim" optimizeCompile="true" csimMode="0" lastCsimMode="0" compiler="true"/>
    </Simulation>
    <files xmlns="">
        <file name="../src/SyncTb.cpp" sc="0" tb="1" cflags=" -std=c++0x"/>
        <file name="rx/src/config.h" sc="0" tb="false" cflags=""/>
        <file name="rx/src/Sync.h" sc="0" tb="false" cflags=""/>
        <file name="rx/src/Sync.cpp" sc="0" tb="false" cflags=""/>
    </files>
    <solutions xmlns="">
        <solution name="solution1" status="active"/>
    </solutions>
</project>

