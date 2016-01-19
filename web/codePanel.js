var accordionID = 0;

function addCodePanel(funcName, enabled) {
  //var idx = $('.codeAccordion').children().length;
  console.log(accordionID);
  console.log(funcName);
  accordionID = getID();
  var butDel='button' + accordionID + 'del';
  var butOn='button' + accordionID + 'a';
  var butOff='button' + accordionID + 'b';
  var accRowID = 'accRow' + accordionID;
  var newDiv = '<div id="' + accRowID + '"><h1><span class="accHeader">' + funcName +
  '</span><div id="onOffRadioSet' + accordionID + '" class="onOffRadios"><button id="' + butDel + '" class="delButton">Delete</button><button data-enabled="1" id="' + butOn + '" class="onButton">On</button><button data-enabled="1" id="' + butOff + '" class="offButton">Off</button>' +
  '</div>' +
  '</h1><div class="editorPanel"></div></div>';
  // var newDiv = '<div><h1><span class="accHeader">' + functionNames.get(i) +
  // '</span><div id="funcdata" data-name="' + functionNames.get(i) +'" style="display:none"></div><div id="onOffRadioSet' + i + '" class="onOffRadios"><button id="' + butOn + '">On</button><button id="' + butOff + '">Off</button>' +
  // '</div>' +
  // '</h1><div class="editorPanel"></div></div>';
  console.log("added div");
  $('.codeAccordion').append(newDiv)
  $("#" + butOn).button().click({"bOn": butOn, "bOff": butOff, "row":accRowID}, function (event) {
    appx.enableFDef($("#" + event.data.row).find("span.accHeader").html(), 1);
    //console.log($("#" + event.data.row).find("span.accHeader").html());
    generate();
    $("#" + event.data.bOn).css({"background":"LimeGreen"});
    $("#" + event.data.bOn).css({"color":"white"});
    $("#" + event.data.bOn).data("enabled","1");
    $("#" + event.data.bOff).css({"background":"white"});
    $("#" + event.data.bOff).css({"color":"black"});
    $("#" + event.data.bOff).data("enabled","0");
    return false;
  })
  .css({"background":"LimeGreen"})
  .css({"color":"white"})
  .data("enabled","1");
  $("#" + butOff).button().click({"bOn": butOn, "bOff": butOff, "row":accRowID}, function (event) {
    console.log(event.data.funcName);
    appx.enableFDef($("#" + event.data.row).find("span.accHeader").html(), 0);
    generate();
    $("#" + event.data.bOff).css({"background":"OrangeRed"});
    $("#" + event.data.bOff).css({"color":"white"});
    $("#" + event.data.bOff).data("enabled","1");
    $("#" + event.data.bOn).css({"background":"white"});
    $("#" + event.data.bOn).css({"color":"black"});
    $("#" + event.data.bOn).data("enabled","0");
    return false;
  })
  .css({"background":"white"})
  .css({"color":"black"})
  .data("enabled","0");
  $("#" + butDel).button().click({"funcName":funcName}, function (event) {
    console.log("del: " + event.data.funcName);
    console.log($('#' + accRowID));
    $('#' + accRowID).remove();
    $('.codeAccordion').accordion("refresh");
    appx.removeFDef(event.data.funcName);
    generate();
    $('.codeAccordion').find(".delButton").css({"visibility":"hidden"});
    return false;
  })
  .css({"background":"white"})
  .css({"color":"red"})
  .css({"visibility":"hidden"});
  if(!enabled) {
    $("#" + butOff).css({"background":"OrangeRed"});
    $("#" + butOff).css({"color":"white"});
    $("#" + butOff).data("enabled","1");
    $("#" + butOn).css({"background":"white"});
    $("#" + butOn).css({"color":"black"});
    $("#" + butOn).data("enabled","0");
  }
  accordionID++;
}
