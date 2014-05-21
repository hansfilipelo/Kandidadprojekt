<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE eagle SYSTEM "eagle.dtd">
<eagle version="6.5.0">
<drawing>
<settings>
<setting alwaysvectorfont="no"/>
<setting verticaltext="up"/>
</settings>
<grid distance="0.1" unitdist="inch" unit="inch" style="lines" multiple="1" display="no" altdistance="0.01" altunitdist="inch" altunit="inch"/>
<layers>
<layer number="1" name="Top" color="4" fill="1" visible="no" active="no"/>
<layer number="16" name="Bottom" color="1" fill="1" visible="no" active="no"/>
<layer number="17" name="Pads" color="2" fill="1" visible="no" active="no"/>
<layer number="18" name="Vias" color="2" fill="1" visible="no" active="no"/>
<layer number="19" name="Unrouted" color="6" fill="1" visible="no" active="no"/>
<layer number="20" name="Dimension" color="15" fill="1" visible="no" active="no"/>
<layer number="21" name="tPlace" color="7" fill="1" visible="no" active="no"/>
<layer number="22" name="bPlace" color="7" fill="1" visible="no" active="no"/>
<layer number="23" name="tOrigins" color="15" fill="1" visible="no" active="no"/>
<layer number="24" name="bOrigins" color="15" fill="1" visible="no" active="no"/>
<layer number="25" name="tNames" color="7" fill="1" visible="no" active="no"/>
<layer number="26" name="bNames" color="7" fill="1" visible="no" active="no"/>
<layer number="27" name="tValues" color="7" fill="1" visible="no" active="no"/>
<layer number="28" name="bValues" color="7" fill="1" visible="no" active="no"/>
<layer number="29" name="tStop" color="7" fill="3" visible="no" active="no"/>
<layer number="30" name="bStop" color="7" fill="6" visible="no" active="no"/>
<layer number="31" name="tCream" color="7" fill="4" visible="no" active="no"/>
<layer number="32" name="bCream" color="7" fill="5" visible="no" active="no"/>
<layer number="33" name="tFinish" color="6" fill="3" visible="no" active="no"/>
<layer number="34" name="bFinish" color="6" fill="6" visible="no" active="no"/>
<layer number="35" name="tGlue" color="7" fill="4" visible="no" active="no"/>
<layer number="36" name="bGlue" color="7" fill="5" visible="no" active="no"/>
<layer number="37" name="tTest" color="7" fill="1" visible="no" active="no"/>
<layer number="38" name="bTest" color="7" fill="1" visible="no" active="no"/>
<layer number="39" name="tKeepout" color="4" fill="11" visible="no" active="no"/>
<layer number="40" name="bKeepout" color="1" fill="11" visible="no" active="no"/>
<layer number="41" name="tRestrict" color="4" fill="10" visible="no" active="no"/>
<layer number="42" name="bRestrict" color="1" fill="10" visible="no" active="no"/>
<layer number="43" name="vRestrict" color="2" fill="10" visible="no" active="no"/>
<layer number="44" name="Drills" color="7" fill="1" visible="no" active="no"/>
<layer number="45" name="Holes" color="7" fill="1" visible="no" active="no"/>
<layer number="46" name="Milling" color="3" fill="1" visible="no" active="no"/>
<layer number="47" name="Measures" color="7" fill="1" visible="no" active="no"/>
<layer number="48" name="Document" color="7" fill="1" visible="no" active="no"/>
<layer number="49" name="Reference" color="7" fill="1" visible="no" active="no"/>
<layer number="50" name="dxf" color="7" fill="1" visible="no" active="no"/>
<layer number="51" name="tDocu" color="7" fill="1" visible="no" active="no"/>
<layer number="52" name="bDocu" color="7" fill="1" visible="no" active="no"/>
<layer number="91" name="Nets" color="2" fill="1" visible="yes" active="yes"/>
<layer number="92" name="Busses" color="1" fill="1" visible="yes" active="yes"/>
<layer number="93" name="Pins" color="2" fill="1" visible="no" active="yes"/>
<layer number="94" name="Symbols" color="4" fill="1" visible="yes" active="yes"/>
<layer number="95" name="Names" color="7" fill="1" visible="yes" active="yes"/>
<layer number="96" name="Values" color="7" fill="1" visible="yes" active="yes"/>
<layer number="97" name="Info" color="7" fill="1" visible="yes" active="yes"/>
<layer number="98" name="Guide" color="6" fill="1" visible="yes" active="yes"/>
<layer number="250" name="Descript" color="3" fill="1" visible="no" active="no"/>
<layer number="251" name="SMDround" color="12" fill="11" visible="no" active="no"/>
</layers>
<schematic xreflabel="%F%N/%S.%C%R" xrefpart="/%S.%C%R">
<libraries>
<library name="1284p">
<packages>
<package name="DIL40">
<description>&lt;b&gt;Dual In Line Package&lt;/b&gt;</description>
<wire x1="25.4" y1="6.731" x2="-25.4" y2="6.731" width="0.1524" layer="21"/>
<wire x1="-25.4" y1="-6.731" x2="25.4" y2="-6.731" width="0.1524" layer="21"/>
<wire x1="25.4" y1="6.731" x2="25.4" y2="-6.731" width="0.1524" layer="21"/>
<wire x1="-25.4" y1="6.731" x2="-25.4" y2="0.889" width="0.1524" layer="21"/>
<wire x1="-25.4" y1="-6.731" x2="-25.4" y2="-1.143" width="0.1524" layer="21"/>
<wire x1="-25.4" y1="0.889" x2="-25.4" y2="-1.143" width="0.1524" layer="21" curve="-180"/>
<pad name="1" x="-24.13" y="-7.62" drill="0.8128" shape="long" rot="R90"/>
<pad name="2" x="-21.59" y="-7.62" drill="0.8128" shape="long" rot="R90"/>
<pad name="7" x="-8.89" y="-7.62" drill="0.8128" shape="long" rot="R90"/>
<pad name="8" x="-6.35" y="-7.62" drill="0.8128" shape="long" rot="R90"/>
<pad name="3" x="-19.05" y="-7.62" drill="0.8128" shape="long" rot="R90"/>
<pad name="4" x="-16.51" y="-7.62" drill="0.8128" shape="long" rot="R90"/>
<pad name="6" x="-11.43" y="-7.62" drill="0.8128" shape="long" rot="R90"/>
<pad name="5" x="-13.97" y="-7.62" drill="0.8128" shape="long" rot="R90"/>
<pad name="9" x="-3.81" y="-7.62" drill="0.8128" shape="long" rot="R90"/>
<pad name="10" x="-1.27" y="-7.62" drill="0.8128" shape="long" rot="R90"/>
<pad name="11" x="1.27" y="-7.62" drill="0.8128" shape="long" rot="R90"/>
<pad name="12" x="3.81" y="-7.62" drill="0.8128" shape="long" rot="R90"/>
<pad name="13" x="6.35" y="-7.62" drill="0.8128" shape="long" rot="R90"/>
<pad name="14" x="8.89" y="-7.62" drill="0.8128" shape="long" rot="R90"/>
<pad name="15" x="11.43" y="-7.62" drill="0.8128" shape="long" rot="R90"/>
<pad name="16" x="13.97" y="-7.62" drill="0.8128" shape="long" rot="R90"/>
<pad name="17" x="16.51" y="-7.62" drill="0.8128" shape="long" rot="R90"/>
<pad name="18" x="19.05" y="-7.62" drill="0.8128" shape="long" rot="R90"/>
<pad name="19" x="21.59" y="-7.62" drill="0.8128" shape="long" rot="R90"/>
<pad name="20" x="24.13" y="-7.62" drill="0.8128" shape="long" rot="R90"/>
<pad name="21" x="24.13" y="7.62" drill="0.8128" shape="long" rot="R90"/>
<pad name="22" x="21.59" y="7.62" drill="0.8128" shape="long" rot="R90"/>
<pad name="23" x="19.05" y="7.62" drill="0.8128" shape="long" rot="R90"/>
<pad name="24" x="16.51" y="7.62" drill="0.8128" shape="long" rot="R90"/>
<pad name="25" x="13.97" y="7.62" drill="0.8128" shape="long" rot="R90"/>
<pad name="26" x="11.43" y="7.62" drill="0.8128" shape="long" rot="R90"/>
<pad name="27" x="8.89" y="7.62" drill="0.8128" shape="long" rot="R90"/>
<pad name="28" x="6.35" y="7.62" drill="0.8128" shape="long" rot="R90"/>
<pad name="29" x="3.81" y="7.62" drill="0.8128" shape="long" rot="R90"/>
<pad name="30" x="1.27" y="7.62" drill="0.8128" shape="long" rot="R90"/>
<pad name="31" x="-1.27" y="7.62" drill="0.8128" shape="long" rot="R90"/>
<pad name="32" x="-3.81" y="7.62" drill="0.8128" shape="long" rot="R90"/>
<pad name="33" x="-6.35" y="7.62" drill="0.8128" shape="long" rot="R90"/>
<pad name="34" x="-8.89" y="7.62" drill="0.8128" shape="long" rot="R90"/>
<pad name="35" x="-11.43" y="7.62" drill="0.8128" shape="long" rot="R90"/>
<pad name="36" x="-13.97" y="7.62" drill="0.8128" shape="long" rot="R90"/>
<pad name="37" x="-16.51" y="7.62" drill="0.8128" shape="long" rot="R90"/>
<pad name="38" x="-19.05" y="7.62" drill="0.8128" shape="long" rot="R90"/>
<pad name="39" x="-21.59" y="7.62" drill="0.8128" shape="long" rot="R90"/>
<pad name="40" x="-24.13" y="7.62" drill="0.8128" shape="long" rot="R90"/>
<text x="-25.908" y="-6.604" size="1.778" layer="25" rot="R90">&gt;NAME</text>
<text x="-17.145" y="-1.016" size="1.778" layer="27">&gt;VALUE</text>
</package>
</packages>
<symbols>
<symbol name="1284">
<wire x1="-17.78" y1="45.72" x2="17.78" y2="45.72" width="0.254" layer="94"/>
<wire x1="17.78" y1="45.72" x2="17.78" y2="-45.72" width="0.254" layer="94"/>
<wire x1="17.78" y1="-45.72" x2="-17.78" y2="-45.72" width="0.254" layer="94"/>
<wire x1="-17.78" y1="-45.72" x2="-17.78" y2="45.72" width="0.254" layer="94"/>
<text x="-15.24" y="45.72" size="1.27" layer="95">&gt;NAME</text>
<text x="10.16" y="45.72" size="1.27" layer="96">&gt;VALUE</text>
<pin name="ADC7/PCINT7" x="22.86" y="25.4" length="middle" rot="R180"/>
<pin name="ADC6/PCINT6" x="22.86" y="27.94" length="middle" rot="R180"/>
<pin name="ADC5/PCINT5" x="22.86" y="30.48" length="middle" rot="R180"/>
<pin name="ADC4/PCINT4" x="22.86" y="33.02" length="middle" rot="R180"/>
<pin name="ADC3/PCINT3" x="22.86" y="35.56" length="middle" rot="R180"/>
<pin name="ADC2/PCINT2" x="22.86" y="38.1" length="middle" rot="R180"/>
<pin name="ADC1/PCINT1" x="22.86" y="40.64" length="middle" rot="R180"/>
<pin name="ADC0/PCINT0" x="22.86" y="43.18" length="middle" rot="R180"/>
<pin name="GND@1" x="-22.86" y="-43.18" length="middle"/>
<pin name="OC2A/PCINT31" x="22.86" y="-43.18" length="middle" rot="R180"/>
<pin name="OC2B/ICP/PCINT30" x="22.86" y="-40.64" length="middle" rot="R180"/>
<pin name="OC1A/PCINT29" x="22.86" y="-38.1" length="middle" rot="R180"/>
<pin name="XCK1/OC1B/PCINT28" x="22.86" y="-35.56" length="middle" rot="R180"/>
<pin name="TXD1/INT1/PCINT27" x="22.86" y="-33.02" length="middle" rot="R180"/>
<pin name="RXD1/INT0/PCINT26" x="22.86" y="-30.48" length="middle" rot="R180"/>
<pin name="T3/RXD0/PCINT24" x="22.86" y="-25.4" length="middle" rot="R180"/>
<pin name="XTAL1" x="-22.86" y="-2.54" length="middle"/>
<pin name="XTAL2" x="-22.86" y="-12.7" length="middle"/>
<pin name="VCC" x="-22.86" y="43.18" length="middle"/>
<pin name="RESET" x="-22.86" y="25.4" length="middle"/>
<pin name="TOSC2/PCINT23" x="22.86" y="-20.32" length="middle" rot="R180"/>
<pin name="TOSC1/PCINT22" x="22.86" y="-17.78" length="middle" rot="R180"/>
<pin name="TDI/PCINT21" x="22.86" y="-15.24" length="middle" rot="R180"/>
<pin name="TDO/PCINT20" x="22.86" y="-12.7" length="middle" rot="R180"/>
<pin name="TMS/PCINT19" x="22.86" y="-10.16" length="middle" rot="R180"/>
<pin name="TCK/PCINT18" x="22.86" y="-7.62" length="middle" rot="R180"/>
<pin name="SDA/PCINT17" x="22.86" y="-5.08" length="middle" rot="R180"/>
<pin name="SCL/PCINT16" x="22.86" y="-2.54" length="middle" rot="R180"/>
<pin name="SCK/OC3B/PCINT15" x="22.86" y="2.54" length="middle" rot="R180"/>
<pin name="MISO/OC3A/PCINT14" x="22.86" y="5.08" length="middle" rot="R180"/>
<pin name="MOSI/ICP3/PCINT13" x="22.86" y="7.62" length="middle" rot="R180"/>
<pin name="SS/OC0B/PCINT12" x="22.86" y="10.16" length="middle" rot="R180"/>
<pin name="AIN1/OC0A/PCINT11" x="22.86" y="12.7" length="middle" rot="R180"/>
<pin name="AIN0/INT2/PCINT10" x="22.86" y="15.24" length="middle" rot="R180"/>
<pin name="T1/CLK0/PCINT9" x="22.86" y="17.78" length="middle" rot="R180"/>
<pin name="T0/XCK0/PCINT8" x="22.86" y="20.32" length="middle" rot="R180"/>
<pin name="GND@0" x="-22.86" y="-38.1" length="middle"/>
<pin name="AVCC" x="-22.86" y="38.1" length="middle"/>
<pin name="AREF" x="-22.86" y="33.02" length="middle"/>
<pin name="TXD0/PCINT25" x="22.86" y="-27.94" length="middle" rot="R180"/>
</symbol>
</symbols>
<devicesets>
<deviceset name="MEGA1284P" prefix="IC" uservalue="yes">
<description>Atmel ATmega1284P 8-bit AVR Microcontroller</description>
<gates>
<gate name="G$1" symbol="1284" x="0" y="0"/>
</gates>
<devices>
<device name="" package="DIL40">
<connects>
<connect gate="G$1" pin="ADC0/PCINT0" pad="40"/>
<connect gate="G$1" pin="ADC1/PCINT1" pad="39"/>
<connect gate="G$1" pin="ADC2/PCINT2" pad="38"/>
<connect gate="G$1" pin="ADC3/PCINT3" pad="37"/>
<connect gate="G$1" pin="ADC4/PCINT4" pad="36"/>
<connect gate="G$1" pin="ADC5/PCINT5" pad="35"/>
<connect gate="G$1" pin="ADC6/PCINT6" pad="34"/>
<connect gate="G$1" pin="ADC7/PCINT7" pad="33"/>
<connect gate="G$1" pin="AIN0/INT2/PCINT10" pad="3"/>
<connect gate="G$1" pin="AIN1/OC0A/PCINT11" pad="4"/>
<connect gate="G$1" pin="AREF" pad="32"/>
<connect gate="G$1" pin="AVCC" pad="30"/>
<connect gate="G$1" pin="GND@0" pad="31"/>
<connect gate="G$1" pin="GND@1" pad="11"/>
<connect gate="G$1" pin="MISO/OC3A/PCINT14" pad="7"/>
<connect gate="G$1" pin="MOSI/ICP3/PCINT13" pad="6"/>
<connect gate="G$1" pin="OC1A/PCINT29" pad="19"/>
<connect gate="G$1" pin="OC2A/PCINT31" pad="21"/>
<connect gate="G$1" pin="OC2B/ICP/PCINT30" pad="20"/>
<connect gate="G$1" pin="RESET" pad="9"/>
<connect gate="G$1" pin="RXD1/INT0/PCINT26" pad="16"/>
<connect gate="G$1" pin="SCK/OC3B/PCINT15" pad="8"/>
<connect gate="G$1" pin="SCL/PCINT16" pad="22"/>
<connect gate="G$1" pin="SDA/PCINT17" pad="23"/>
<connect gate="G$1" pin="SS/OC0B/PCINT12" pad="5"/>
<connect gate="G$1" pin="T0/XCK0/PCINT8" pad="1"/>
<connect gate="G$1" pin="T1/CLK0/PCINT9" pad="2"/>
<connect gate="G$1" pin="T3/RXD0/PCINT24" pad="14"/>
<connect gate="G$1" pin="TCK/PCINT18" pad="24"/>
<connect gate="G$1" pin="TDI/PCINT21" pad="27"/>
<connect gate="G$1" pin="TDO/PCINT20" pad="26"/>
<connect gate="G$1" pin="TMS/PCINT19" pad="25"/>
<connect gate="G$1" pin="TOSC1/PCINT22" pad="28"/>
<connect gate="G$1" pin="TOSC2/PCINT23" pad="29"/>
<connect gate="G$1" pin="TXD0/PCINT25" pad="15"/>
<connect gate="G$1" pin="TXD1/INT1/PCINT27" pad="17"/>
<connect gate="G$1" pin="VCC" pad="10"/>
<connect gate="G$1" pin="XCK1/OC1B/PCINT28" pad="18"/>
<connect gate="G$1" pin="XTAL1" pad="13"/>
<connect gate="G$1" pin="XTAL2" pad="12"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
</devices>
</deviceset>
</devicesets>
</library>
<library name="supply2">
<description>&lt;b&gt;Supply Symbols&lt;/b&gt;&lt;p&gt;
GND, VCC, 0V, +5V, -5V, etc.&lt;p&gt;
Please keep in mind, that these devices are necessary for the
automatic wiring of the supply signals.&lt;p&gt;
The pin name defined in the symbol is identical to the net which is to be wired automatically.&lt;p&gt;
In this library the device names are the same as the pin names of the symbols, therefore the correct signal names appear next to the supply symbols in the schematic.&lt;p&gt;
&lt;author&gt;Created by librarian@cadsoft.de&lt;/author&gt;</description>
<packages>
</packages>
<symbols>
<symbol name="GND">
<wire x1="-1.27" y1="0" x2="1.27" y2="0" width="0.254" layer="94"/>
<wire x1="1.27" y1="0" x2="0" y2="-1.27" width="0.254" layer="94"/>
<wire x1="0" y1="-1.27" x2="-1.27" y2="0" width="0.254" layer="94"/>
<text x="-1.905" y="-3.175" size="1.778" layer="96">&gt;VALUE</text>
<pin name="GND" x="0" y="2.54" visible="off" length="short" direction="sup" rot="R270"/>
</symbol>
<symbol name="+05V">
<wire x1="-0.635" y1="1.27" x2="0.635" y2="1.27" width="0.1524" layer="94"/>
<wire x1="0" y1="0.635" x2="0" y2="1.905" width="0.1524" layer="94"/>
<circle x="0" y="1.27" radius="1.27" width="0.254" layer="94"/>
<text x="-1.905" y="3.175" size="1.778" layer="96">&gt;VALUE</text>
<pin name="+5V" x="0" y="-2.54" visible="off" length="short" direction="sup" rot="R90"/>
</symbol>
</symbols>
<devicesets>
<deviceset name="GND" prefix="SUPPLY">
<description>&lt;b&gt;SUPPLY SYMBOL&lt;/b&gt;</description>
<gates>
<gate name="GND" symbol="GND" x="0" y="0"/>
</gates>
<devices>
<device name="">
<technologies>
<technology name=""/>
</technologies>
</device>
</devices>
</deviceset>
<deviceset name="+5V" prefix="SUPPLY">
<description>&lt;b&gt;SUPPLY SYMBOL&lt;/b&gt;</description>
<gates>
<gate name="+5V" symbol="+05V" x="0" y="0"/>
</gates>
<devices>
<device name="">
<technologies>
<technology name=""/>
</technologies>
</device>
</devices>
</deviceset>
</devicesets>
</library>
</libraries>
<attributes>
</attributes>
<variantdefs>
</variantdefs>
<classes>
<class number="0" name="default" width="0" drill="0">
</class>
</classes>
<parts>
<part name="IC1" library="1284p" deviceset="MEGA1284P" device=""/>
<part name="SUPPLY1" library="supply2" deviceset="GND" device=""/>
<part name="SUPPLY2" library="supply2" deviceset="+5V" device=""/>
</parts>
<sheets>
<sheet>
<plain>
<text x="-73.66" y="-68.58" size="5.08" layer="91">Buss</text>
<text x="55.88" y="66.04" size="1.778" layer="91">Avståndssensorer genom LP-filter</text>
<text x="35.56" y="-25.4" size="1.778" layer="91" rot="R270">RFID-sensor</text>
<text x="78.74" y="40.64" size="1.778" layer="91" rot="R270">Vinkelhastighetssensor genom LP-filter</text>
<text x="25.4" y="48.26" size="2.54" layer="91">Analoga signaler</text>
<text x="-50.8" y="22.86" size="1.778" layer="91">avstudsad manuell switch</text>
<text x="-50.8" y="-5.08" size="1.778" layer="91">Kl från master</text>
<text x="30.48" y="15.24" size="1.778" layer="91">Interrupt to master</text>
<text x="30.48" y="10.16" size="1.778" layer="91">Slave select: Master</text>
<text x="22.86" y="0" size="1.778" layer="91">Klocka från master</text>
<text x="99.06" y="-7.62" size="2.54" layer="91" rot="MR270">JTAG</text>
<text x="27.94" y="66.04" size="1.778" layer="91">Reflexsensor</text>
</plain>
<instances>
<instance part="IC1" gate="G$1" x="0" y="0"/>
<instance part="SUPPLY1" gate="GND" x="-43.18" y="-45.72"/>
<instance part="SUPPLY2" gate="+5V" x="-43.18" y="50.8"/>
</instances>
<busses>
<bus name="B$1">
<segment>
<wire x1="60.96" y1="-63.5" x2="-55.88" y2="-63.5" width="0.762" layer="92"/>
</segment>
</bus>
<bus name="B$2">
<segment>
<wire x1="-55.88" y1="-71.12" x2="60.96" y2="-71.12" width="0.762" layer="92"/>
</segment>
</bus>
</busses>
<nets>
<net name="RE" class="0">
<segment>
<pinref part="IC1" gate="G$1" pin="ADC0/PCINT0"/>
<wire x1="22.86" y1="43.18" x2="53.34" y2="43.18" width="0.1524" layer="91"/>
<wire x1="53.34" y1="43.18" x2="53.34" y2="58.42" width="0.1524" layer="91"/>
<wire x1="53.34" y1="58.42" x2="33.02" y2="58.42" width="0.1524" layer="91"/>
<wire x1="33.02" y1="58.42" x2="33.02" y2="63.5" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$2" class="0">
<segment>
<pinref part="IC1" gate="G$1" pin="MOSI/ICP3/PCINT13"/>
<wire x1="22.86" y1="7.62" x2="50.8" y2="7.62" width="0.1524" layer="91"/>
<wire x1="50.8" y1="7.62" x2="50.8" y2="-63.5" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$3" class="0">
<segment>
<pinref part="IC1" gate="G$1" pin="MISO/OC3A/PCINT14"/>
<wire x1="22.86" y1="5.08" x2="45.72" y2="5.08" width="0.1524" layer="91"/>
<wire x1="45.72" y1="5.08" x2="45.72" y2="-71.12" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$4" class="0">
<segment>
<pinref part="IC1" gate="G$1" pin="ADC1/PCINT1"/>
<wire x1="22.86" y1="40.64" x2="55.88" y2="40.64" width="0.1524" layer="91"/>
<wire x1="55.88" y1="40.64" x2="55.88" y2="63.5" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$5" class="0">
<segment>
<pinref part="IC1" gate="G$1" pin="ADC2/PCINT2"/>
<wire x1="22.86" y1="38.1" x2="58.42" y2="38.1" width="0.1524" layer="91"/>
<wire x1="58.42" y1="38.1" x2="58.42" y2="63.5" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$6" class="0">
<segment>
<pinref part="IC1" gate="G$1" pin="ADC3/PCINT3"/>
<wire x1="22.86" y1="35.56" x2="60.96" y2="35.56" width="0.1524" layer="91"/>
<wire x1="60.96" y1="35.56" x2="60.96" y2="63.5" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$7" class="0">
<segment>
<pinref part="IC1" gate="G$1" pin="ADC4/PCINT4"/>
<wire x1="22.86" y1="33.02" x2="63.5" y2="33.02" width="0.1524" layer="91"/>
<wire x1="63.5" y1="33.02" x2="63.5" y2="63.5" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$8" class="0">
<segment>
<pinref part="IC1" gate="G$1" pin="ADC5/PCINT5"/>
<wire x1="22.86" y1="30.48" x2="66.04" y2="30.48" width="0.1524" layer="91"/>
<wire x1="66.04" y1="30.48" x2="66.04" y2="63.5" width="0.1524" layer="91"/>
</segment>
</net>
<net name="GND" class="0">
<segment>
<pinref part="IC1" gate="G$1" pin="GND@1"/>
<pinref part="SUPPLY1" gate="GND" pin="GND"/>
<wire x1="-22.86" y1="-43.18" x2="-43.18" y2="-43.18" width="0.1524" layer="91"/>
<wire x1="-43.18" y1="-43.18" x2="-43.18" y2="-38.1" width="0.1524" layer="91"/>
<pinref part="IC1" gate="G$1" pin="GND@0"/>
<wire x1="-43.18" y1="-38.1" x2="-22.86" y2="-38.1" width="0.1524" layer="91"/>
</segment>
</net>
<net name="+5V" class="0">
<segment>
<pinref part="IC1" gate="G$1" pin="VCC"/>
<wire x1="-22.86" y1="43.18" x2="-43.18" y2="43.18" width="0.1524" layer="91"/>
<pinref part="SUPPLY2" gate="+5V" pin="+5V"/>
<wire x1="-43.18" y1="43.18" x2="-43.18" y2="48.26" width="0.1524" layer="91"/>
<wire x1="-20.32" y1="38.1" x2="-43.18" y2="38.1" width="0.1524" layer="91"/>
<wire x1="-43.18" y1="38.1" x2="-43.18" y2="33.02" width="0.1524" layer="91"/>
<pinref part="IC1" gate="G$1" pin="AREF"/>
<wire x1="-22.86" y1="33.02" x2="-43.18" y2="33.02" width="0.1524" layer="91"/>
<wire x1="-43.18" y1="38.1" x2="-43.18" y2="43.18" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$14" class="0">
<segment>
<pinref part="IC1" gate="G$1" pin="RESET"/>
<wire x1="-22.86" y1="25.4" x2="-50.8" y2="25.4" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$15" class="0">
<segment>
<wire x1="-20.32" y1="-2.54" x2="-50.8" y2="-2.54" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$16" class="0">
<segment>
<pinref part="IC1" gate="G$1" pin="AIN0/INT2/PCINT10"/>
<wire x1="22.86" y1="15.24" x2="50.8" y2="15.24" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$13" class="0">
<segment>
<pinref part="IC1" gate="G$1" pin="SS/OC0B/PCINT12"/>
<wire x1="22.86" y1="10.16" x2="50.8" y2="10.16" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$17" class="0">
<segment>
<pinref part="IC1" gate="G$1" pin="SCK/OC3B/PCINT15"/>
<wire x1="22.86" y1="2.54" x2="43.18" y2="2.54" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$9" class="0">
<segment>
<wire x1="22.86" y1="25.4" x2="73.66" y2="25.4" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$11" class="0">
<segment>
<pinref part="IC1" gate="G$1" pin="ADC6/PCINT6"/>
<wire x1="22.86" y1="27.94" x2="68.58" y2="27.94" width="0.1524" layer="91"/>
<wire x1="68.58" y1="27.94" x2="68.58" y2="63.5" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$10" class="0">
<segment>
<pinref part="IC1" gate="G$1" pin="T3/RXD0/PCINT24"/>
<wire x1="22.86" y1="-25.4" x2="30.48" y2="-25.4" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$12" class="0">
<segment>
<pinref part="IC1" gate="G$1" pin="TCK/PCINT18"/>
<wire x1="22.86" y1="-7.62" x2="93.98" y2="-7.62" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$18" class="0">
<segment>
<pinref part="IC1" gate="G$1" pin="TMS/PCINT19"/>
<wire x1="22.86" y1="-10.16" x2="93.98" y2="-10.16" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$20" class="0">
<segment>
<pinref part="IC1" gate="G$1" pin="TDI/PCINT21"/>
<wire x1="22.86" y1="-15.24" x2="93.98" y2="-15.24" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$19" class="0">
<segment>
<pinref part="IC1" gate="G$1" pin="TDO/PCINT20"/>
<wire x1="22.86" y1="-12.7" x2="93.98" y2="-12.7" width="0.1524" layer="91"/>
</segment>
</net>
</nets>
</sheet>
</sheets>
</schematic>
</drawing>
</eagle>
