(
s.boot;
s.scope;
s.freqscope;
)

(
MIDIClient.disposeClient;
MIDIClient.init;
MIDIIn.connectAll;
MIDIClient.sources;
)

MIDIClient.sources.do(_.postln);
MIDIFunc.trace(true);

// function for wave terrain example
f = { |x, y, a, b| (b * sin((x*sin(x) - (y*sin(y) * log(x * x + 1))) / a)) + ((1 - b) * (exp(-1 * (x * x + y * y) * 0.15) * sin(sqrt(x * x + y * y) * a * 0.4)))};


//Suono pulito
(
x = {
 var r = MouseX.kr(0, 10);
 var a = (LFSaw.kr(MouseY.kr(0.05, 50)).range(1, 10));
 var b = MouseButton.kr(0, 1, 0.8).round(0.01).poll;
 // stereo expansion
 var phi = LFSaw.ar([80, 80.1], -1).range(0, 2pi);
 var x = r * cos(phi);
 var y = r * sin(phi);
 Limiter.ar(f.(x, y, a, b), 0.2)
}.play
)

(
SynthDef("terrain", {
 |out, r , a, b|
 var phi, x, y, arp;

 arp = ((LFSaw.kr(a).range(1, 10)));

 // stereo expansion
 phi = LFSaw.ar([320, 160.1], -1).range(0, 2pi);
 x = r * cos(phi);
 y = r * sin(phi);
 Out.ar(out, Limiter.ar(f.(x, y, arp, b), 0.2));
}).add;
)

(
t = Synth("terrain");

MIDIdef.cc(\midiR, {
 |val, num, chan, src|
 var scaled = val / 127.0;
 t.set(\r, scaled * 10);
}, ccNum: 7);

MIDIdef.cc(\midiA, {
 |val, num, chan, src|
 var scaled = val / 127.0;
 t.set(\a, 0.05 + scaled * 49.95 );
}, ccNum: 6);

MIDIdef.cc(\midiB, {
 |val, num, chan, src|
 var scaled = val / 127.0;
 t.set(\b, scaled);
}, ccNum: 5);

)

t.set(\b, 1);

x.release