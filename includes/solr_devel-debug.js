/**
 * @file
 * Solr_devel module helpers.
 */

var $ = jQuery;

Drupal.behaviors.myBehavior = {
    attach: function (context, settings) {
      $(context).find("div.solr-devel-debug").not(".expanded").find("div.solr-devel-debug-content").hide();
      $(context).find("div.solr-devel-debug div.solr-devel-debug-title").click(function () {
        if (!$(this).parent().children("div.solr-devel-debug-content").is(":visible")) {
          $(this).parent().addClass("expanded");
        }
        else {
          $(this).parent().removeClass("expanded");
        }
        $(this).parent().children("div.solr-devel-debug-content").toggle();
      })
      
      $(context).find("div.solr-devel-debug .krumo-expand").addClass("krumo-opened");
      $(context).find("div.solr-devel-debug .krumo-nest").show();
      
      $("div.messages > ul > li > *").insertAfter ("#messages .messages");
      
      $("div.messages > ul > li").each(function () {
        if ($(this)[0].childNodes.length == 0) {
          $(this).remove();
        }
      })

      if ($("div.messages > ul").children().length == 0) {
        $("div.messages").remove();
      }
    }
  };
