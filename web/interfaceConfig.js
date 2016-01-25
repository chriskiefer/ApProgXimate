function prepareInterface() {


  $("#closeBrowserPopupButton").button()
  .click(function() {
    $("#browserCheckPopup").popup('hide');
  });
  $("#includeDocs").load("docs_listenmode.html", function() {
  });
  $("#includeFAQ").load("FAQ.html");
  $("#includeRef").load("Ref.html");
  $("#includeAbout").load("about.html");

  $("#topButtonBar").hide();
  $("#listenButtonBar").show();
  $("#RefButton").hide();
  $("#realtimeErrorMessage").hide();
  $("#syntaxErrorMessage").hide();
  $("#listenButton").button()
  .click(function() {
    interfaceMode = interfaceModes.LISTEN;
    apxlayout.toggle("east");
    apxlayout.toggle("west");
    $("#topButtonBar").hide();
    $("#listenButtonBar").show();
    $("#RefButton").hide();
    $("#includeDocs").load("docs_listenmode.html", function() {
    });
    backupParams();
  });
  $("#createButton").button()
  .click(function() {
    interfaceMode = interfaceModes.CREATE;
    apxlayout.toggle("east");
    apxlayout.toggle("west");
    $("#topButtonBar").show();
    $("#listenButtonBar").hide();
    $("#RefButton").show();
    $("#includeDocs").load("docs.html", function() {
    });
    $('.codeAccordion').accordion("refresh");
  });
  $("#helpButton").button()
  .click(function() {
    Prism.highlightAll();
    $('#my_popup').popup({
      autoopen:true,
      color:'white',
      outline:true,
      transition: 'all 0.3s'
    });
  });
  $("#helpCloseButton").button()
  .click(function() {
    $("#my_popup").popup('hide');
  });
  $("#FAQButton").button()
  .click(function() {
    $('#FAQPopup').popup({
      autoopen:true,
      color:'white',
      outline:true,
      transition: 'all 0.3s'
    });
  });
  $("#FAQCloseButton").button()
  .click(function() {
    $("#FAQPopup").popup('hide');
  });
  $("#AboutButton").button()
  .click(function() {
    $('#AboutPopup').popup({
      autoopen:true,
      color:'white',
      outline:true,
      transition: 'all 0.3s'
    });
  });
  $("#AboutCloseButton").button()
  .click(function() {
    $("#AboutPopup").popup('hide');
  });
  $("#RefButton").button()
  .click(function() {
    Prism.highlightAll();
    $('#RefPopup').popup({
      autoopen:true,
      color:'white',
      outline:true,
      transition: 'all 0.3s'
    });
  });
  $("#RefCloseButton").button()
  .click(function() {
    $("#RefPopup").popup('hide');
  });

}
