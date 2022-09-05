$(function(){
  $(".table_tree .fold .span_ico").click(function(){
    var ftr=$(this).closest("tr");
    var nid=ftr.attr("nid");
    var ctr=$(".table_tree tr."+nid);
    var cctr=$(".table_tree tr.tr_hidden").filter("[nid^='"+nid+"']").filter("[nid!='"+nid+"']");
    ftr.toggleClass("collapsed").toggleClass("expanded");
    if(ftr.is(".expanded")){
      ctr.show();
    }else{
      cctr.hide().filter(".fold").removeClass("expanded").addClass("collapsed");
    }
  });
});