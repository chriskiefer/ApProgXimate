function linlin(j0,j1,j2) {
  return Module.maxiMap.linlin(j0, -1, 1, j1, j2);
}
function linexp(j0,j1,j2) {
  return Module.maxiMap.linexp(j0, -1, 1, j1, j2);
}
function seq() {
  function next(trig) {

  }
}
function loadDefaultFunctions() {
  appx.addFuncDef("add", "function add() {\n \
    this.play = function(j1, j2) {\n \
      return j1 + j2;\n \
    }\n \
  }\n", 2, 99999);
  appx.addFuncDef("mix", "function mix() {\n \
    this.play = function(j1, j2, j3) {\n \
      return (j1 * j3) + (j2 * (1 - j3));\n \
    }\n \
  }\n", 3, 99999);
  appx.addFuncDef("mul", "function mul() {\n \
    this.play = function(j1, j2) {\n \
      return j1 * j2;\n \
    }\n \
  }\n", 2, 99999);

  appx.addFuncDef("saw", "function saw() {\n \
    this.osc = new Module.maxiOsc();\n \
    this.play = function(j0) {\n \
      return this.osc.sawn(Module.maxiMap.linexp(j0,-1,1,20,20000));\n \
    }\n \
  }\n", 1, 99999);

  appx.addFuncDef("lfsaw", "function lfsaw() {\n \
    this.osc = new Module.maxiOsc();\n \
    this.play = function(j0) {\n \
      return this.osc.saw(Module.maxiMap.linexp(j0,-1,1,0.01,15));\n \
    }\n \
  }\n", 1, 99999);

  //   appx.addFuncDef("square", "function square() {\n \
  //     this.osc = new Module.maxiOsc();\n \
  //     this.play = function(j0) {\n \
  //       return this.osc.square(Module.maxiMap.linexp(j0,-1,1,20,20000));\n \
  //     }\n \
  //   }\n", 1, 99999);
  //
    appx.addFuncDef("lfsquare", "function lfsquare() {\n \
      this.osc = new Module.maxiOsc();\n \
      this.play = function(j0) {\n \
        return this.osc.square(Module.maxiMap.linexp(j0,-1,1,0.01,15));\n \
      }\n \
    }\n", 1, 99999);
  //
  //   appx.addFuncDef("lpf", "function lpf() {\n \
  //     this.filter = new Module.maxiFilter();\n \
  //     this.play = function(j0,j1) {\n \
  //       return this.filter.lopass(j0, (j1 + 1) / 2);\n \
  //     }\n \
  //   }\n", 2, 99999);
    // appx.addFuncDef("lpfz", "function lpfz() {\n \
    //    this.filter = new Module.maxiFilter();\n \
    //    this.play = function(j0,j1,j2) {\n \
    //        return this.filter.lores(j0, Module.maxiMap.linexp(j1,-1,1,20,10000), Module.maxiMap.linlin(j2, -1, 1, 1, 5));\n \
    //    }\n \
    // }\n", 3, 99999);
  //
    appx.addFuncDef("hpf", "function hpf() {\n \
       this.filter = new Module.maxiFilter();\n \
       this.play = function(j0,j1) {\n \
           return this.filter.hipass(j0, (j1 + 1) / 2);\n \
       }\n \
    }\n", 2, 99999);
  //
  //   appx.addFuncDef("gate", "function gate() {\n \
  //     this.filter = new Module.maxiFilter();\n \
  //     this.ampctl = 0; \
  //     this.play = function(j0,j1, j2) {\n \
  //       var w; \
  //       w = this.filter.lopass(j1 > j2 ? 1 : 0 , 0.5); \
  //       return w * j0;\n \
  //     }\n \
  //   }\n", 3, 99999);
  //
  //
    appx.addFuncDef("tanh_", "function tanh_() {\n \
      this.play = function(j0) {\n \
        return Math.tanh(j0);\n \
      }\n \
    }\n", 1, 99999);
  //
  //   appx.addFuncDef("samp", "\
  // function samp() { \n\
  // 	this.dsp = new Module.maxiSample();  \n\
  // 	this.dsp.load('samples/distortedkick10.wav',0);\n\
  //  	this.play = function(j0) \n{ \
  //  		return this.dsp.play(1.0); \n\
  //  	} \n\
  //  }",1,99999)

  // appx.addFuncDef("gwrite", "function gwrite() {\n \
  //    this.play = function(j0, j1) {\n \
  //        var index; \
  //        index = Math.floor(j0 * 3);\
  //        globalMemory[index] = Math.tanh(globalMemory[index] + j1); \
  //        return j0;\n \
  //    }\n \
  // }\n", 2, 99999);
  //
  // appx.addFuncDef("gread", "function gread() {\n \
  //    this.play = function(j0) {\n \
  //        var index; \
  //        index = Math.floor(j0 * 3);\
  //        return globalMemory[index];\n \
  //    }\n \
  // }\n", 1, 99999);
  appx.addFuncDef("gwr1", "function gwr1() {\n \
    this.play = function(j0) {\n \
      globalMemory[0] = Math.tanh(globalMemory[0] + (j0)); \
      return j0;\n \
    }\n \
  }\n", 1, 99999);

  appx.addFuncDef("gr1", "function gr1() {\n \
    this.play = function(j0) {\n \
      return globalMemory[0] * j0;\n \
    }\n \
  }\n", 1, 99999);
  //
  appx.addFuncDef("seq", "function seq() {\n \
    this.vals = [1,9,4,-6,3]; \
    this.idx = 0; \
    this.last =0; \
     this.play = function(j0) {\n \
         if (this.last < 0 && j0 >= 0) {\
           this.idx++; \
           if (this.idx == this.vals.length) this.idx = 0; \
         } \
         this.last = j0; \
         return this.vals[this.idx] / 10.0;\n \
     }\n \
  }\n", 1, 99999);
}
