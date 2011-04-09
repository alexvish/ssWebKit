jQuery(function($){
        
        // modernizr lite via https://gist.github.com/598008
        var testStyle = function(style) {

          var elem = document.createElement('div');
          var prefixes = ['Webkit', 'Moz', 'O', 'ms', 'Khtml'];
          var bool;
          var bump = function(all, letter) {
                return letter.toUpperCase();
              };
          var prop;

          bool = style in elem.style;
          prop = style.replace(/^(.)/, bump).replace(/-([a-z])/ig, bump);

          for (var len = prefixes.length; len--; ){
            if (bool) { 
              break; 
            }
            bool = prefixes[len] + prop in elem.style;
          }

          document.documentElement.className += ' ' + (bool ? '' : 'no-') + style.replace(/-/g, '');
          return bool;
        };

        var canTransition = testStyle('transition');
        
        /* Thank you, Tim Down
         * http://stackoverflow.com/questions/298750/how-do-i-select-text-nodes-with-jquery
         */
        var getTextNodesIn = function (node, includeWhitespaceNodes) {
            var textNodes = [], whitespace = /^\s*$/;

            function getTextNodes(node) {
                if (node.nodeType == 3) {
                    if (includeWhitespaceNodes || !whitespace.test(node.nodeValue)) {
                        textNodes.push(node.nodeValue);
                    }
                } else {
                    for (var i = 0, len = node.childNodes.length; i < len; ++i) {
                        getTextNodes(node.childNodes[i]);
                    }
                }
            }

            getTextNodes(node);
            return textNodes;
        };
        
        //
        // Slide class
        //
        var Slide = function(node,index) {
          this._node = node;
          if (this._node) {
            $(this._node).addClass('slide distant-slide');
          }
          this._titleElem = $('.title',this._node).first()[0];
          this._title = this._titleElem?getTextNodesIn(this._titleElem,false).join(" "):'Slide ' + (index + 1);
          var me = this;
          this._node.addEventListener('webkitTransitionEnd',function(){me.endOfTransition();},false);
        };

    
        Slide.prototype = {
          _node: null,
          _currentState: '',
          _states: [ 'distant-slide','far-past',
                     'past', 'current', 'future',
                     'far-future','distant-slide' ],
          _titleElem:0,
          _title:0,
          setState: function(state) {
            if (typeof state != 'string') {
              state = this._states[state];
            }

            $(this._node).removeClass(this._states.join(" "));
            $(this._node).addClass(state);
            this._currentState = state;

            if (state == 'current') {
              $('#presentation-counter').html(this._title);
              this._onLoad();
            } else {
              (this._titleElem) && (this._titleElem.style.display='none');
              this._onUnload();
            }
          },
          _onLoad: function() {
            this._fireEvent('onload');
            this._showFrames();
          },
          _onUnload: function() {
            this._fireEvent('onunload');
            this._hideFrames();
          },
          _fireEvent: function(name) {
            var eventSrc = this._node.getAttribute(name);
            if (eventSrc) {
              eventSrc = '(function() { ' + eventSrc + ' })';
              var fn = eval(eventSrc);
              fn.call(this._node);
            }
          },
          _showFrames: function() {
            var frames = $('iframe', this._node);
            function show() {
              frames.each(function() {
                var _src = $(this).attr('_src');
                if (_src && _src.length) {
                  $(this).attr('src', _src);
                }
              });
            }
            setTimeout(show, 0);
          },
          _hideFrames: function() {
            var frames = $('iframe', this._node);
            function hide() {
              frames.each(function() {
                var _src = $(this).attr('_src');
                if (_src && _src.length) {
                  $(this).attr('src', '');
                }
              });
            }
            setTimeout(hide, 250);
          },
          endOfTransition: function() {
             if(this._titleElem && this._currentState == 'current') {
                this._titleElem.style.display='block';                
             }
          }
        };

        //
        // SlideShow class
        //
        var SlideShow = function() {
          _self = this;
          this._slides = $('.slide').map(function(index,slideElem) {
            return new Slide(slideElem,index);
          });
          var firstSlideId = $('.slide').first().attr('id');
          
          var h = window.location.hash;
          try {
            this.current = h;
          } catch (e) { /* squeltch */ }
          this.current = (!this.current) ? firstSlideId : this.current.replace('#', '');
          if (!$('#' + this.current).length) {
            this.current = firstSlideId;
          }
          this._update();
          var _t = this;
          var goNext = function() {
            _t.next();
            setTimeout(goNext,_t.timeout);
          }
          setTimeout(goNext,_t.timeout);
        };

        SlideShow.prototype = {
          _presentationCounter: $('#presentation-counter'),
          _slides: [],
          
          _getCurrentIndex: function() {
            return $('.slide').index($('#' + this.current));
          },
          _update: function(dontPush) {
            var currentIndex = this._getCurrentIndex();
            if (currentIndex < 0) {
                return;
            }
            if (history.pushState) {
              if (!dontPush) {
                history.replaceState(this.current, 'Slide ' + this.current, '#' + this.current);
              }
            } else {
              window.location.hash = this.current;
            }
            this._slides[currentIndex].setState(3);
            for(var i = 1; i < 4; i++) {
               l = currentIndex - i, r = currentIndex + i, lset=false, rset = false;
               (l >=0 && l < this._slides.length) && (this._slides[l].setState(3-i),lset=true);
               (r >=0 && r < this._slides.length) && (this._slides[r].setState(3+i),rset=true);
               if (lset && rset) {
                  continue;
               }
               (l < 0) && (l = this._slides.length + l);
               (r >= this._slides.length) && (r = r % this._slides.length);
               if (l == r) {
                  break;
               }
               (!lset) && this._slides[l].setState(3-i);
               (!rset) && this._slides[r].setState(3+i);
            }
          },

          current: 0,
          timeout: 5000,
          next: function() {
            var next = $('#' + this.current + ' + .slide');
            (next.length > 0) || (next = $('.slide').first());
            if (next.length <= 0) {
                return;
            }
            this.current = next.attr("id");
            this._update();
          }
        };
                
        // Initialize
        var slideshow = new SlideShow();
        
        $('.slides').css('display', 'block');
    
})