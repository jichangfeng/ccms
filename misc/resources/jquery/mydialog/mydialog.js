var myDialog = function(option, div){
    /*
    option:
        text:填充内容
        type:0 1 2 3信息提示 自动退出 包装元素 突出显示元素
    */
    var tipMaxWidth = 450;
    var speed = 200;
    var autoExitSpeed = 1000;
    
    if(!option) option={};
    option.type = option.type || 0;
    if(option.closeOnESCAPE == undefined) option.closeOnESCAPE = true;
    if(option.type*1 == 3){option.closeOnESCAPE = false;}
    
    // add dialog
    var dialog;
    if(option.type*1 == 2 || option.type*1 == 3){
        dialog = $(div[0]);
    }else{
        if(typeof option.text != 'undefined'){
            option.text = option.text.replace(/\[BACKURL\]/, encodeURIComponent(encodeURIComponent(window.location.href)));
        }
        dialog = $('<div></div>').html(option.text || '').appendTo(document.body);
    }
    if(option.closeBtn){
        dialog.append("<div class='closeDiv'><button class='closeBtn closeDialog'>关闭</button></div>");
    }
    if(option.type*1 == 2){
        dialog.addClass("myDialog").fadeIn(speed);
    }else if(option.type*1 == 3){
        dialog.css({position:"relative", "z-index":"2"});
    }else{
        dialog.addClass("myDialogTip").fadeIn(speed);
        if(dialog.width()>tipMaxWidth){dialog.width(tipMaxWidth)}
    }
    
    // add overlay
    var height = helper.height().match(/\d+/)[0];
    var divheight = dialog.height();
    if(height < divheight+10){
        height = divheight+40;
    }
    var opacity = 0.8;
    if(option.opacity != undefined){ opacity = option.opacity; }
    var overlay = $('<div></div>').appendTo(document.body).addClass('mydialog-overlay').css({
        width: helper.width(),
        height: height,
        opacity:opacity
    });
//    if(option.opacity =! undefined){ overlay.css({"opacity": option.opacity});
//        alert(overlay.css("opacity"));}else{
//        overlay.css("opacity",0.8);
//    }
    if(option.type*1 == 3){
        overlay.css("z-index", 1);
    }
    
    overlay.hide().fadeIn(speed);
//    if(option.bgiframe)
	try{
	overlay.bgiframe();
	}catch (e){}
    
    // position
    if(option.type*1 != 3){
        var wnd = $(window), doc = $(document),
    			pTop = doc.scrollTop(), pLeft = doc.scrollLeft(),
    			minTop = pTop;
        var top = (wnd.height()-dialog.height())/2+pTop;
        var left = (wnd.width()-dialog.width())/2+pLeft;
        if(left<=0) left = 10;
        if(top<=0) top = 10;
        dialog.css('left', left).css('top', top);
    }
    if(option.type*1 == 1){
        setTimeout(function(){exit();}, autoExitSpeed);
        return;
    }
    if(option.closeOnClick){
        overlay.click(function(){exit()});
    }
    if(option.closeOnESCAPE){
        $(document).bind('keydown.dialog-overlay', function(event) {
            if(event.keyCode && event.keyCode == 27){
                //event.preventDefault();
                exit();
            }
        });
    }
    dialog[0].exit = exit;
    dialog.find(".closeDialog").click(function(){dialog[0].exit();});
    // utilitise
    function exit(){
        overlay.fadeOut(speed);
        if(option.type*1 == 3){
            dialog.css({position:'', "z-index":''});
        }else{
            dialog.fadeOut(speed);
        }
        setTimeout(function(){overlay.remove(); if(option.type*1<2) dialog.remove()}, speed);
        $([document, window]).unbind('.dialog-overlay');
		!!option.closeFn&&option.closeFn();
    }
    return dialog;
}
var myDialogClose = function(div){
    if(div instanceof Object)
        div[0].exit();
    else
        $(div)[0].exit();
}

var helper ={
	height: function() {
		// handle IE 6
		if ($.browser.msie && $.browser.version < 7) {
			var scrollHeight = Math.max(
				document.documentElement.scrollHeight,
				document.body.scrollHeight
			);
			var offsetHeight = Math.max(
				document.documentElement.offsetHeight,
				document.body.offsetHeight
			);

			if (scrollHeight < offsetHeight) {
				return $(window).height() + 'px';
			} else {
				return scrollHeight + 'px';
			}
		// handle "good" browsers
		} else {
			return $(document).height() + 'px';
		}
	},

	width: function() {
		// handle IE 6
		if ($.browser.msie && $.browser.version < 7) {
			var scrollWidth = Math.max(
				document.documentElement.scrollWidth,
				document.body.scrollWidth
			);
			var offsetWidth = Math.max(
				document.documentElement.offsetWidth,
				document.body.offsetWidth
			);

			if (scrollWidth < offsetWidth) {
				return $(window).width() + 'px';
			} else {
				return scrollWidth + 'px';
			}
		// handle "good" browsers
		} else {
			return $(document).width() + 'px';
		}
	}
}
