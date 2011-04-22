/**
 * Copyright 2011 Alexey Vishentsev.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 * 
 */
 
 
 
 
 
 
 
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
        
        
        //
        // Slide class
        //
        var Slide = function(node,index) {
          this._node = node;
          if (this._node) {
            $(this._node).addClass('slide distant-slide');
          }
          this._titleElem = $('.title',this._node).eq(0);
          this._title = (this._titleElem && this._titleElem.length)?$(this._titleElem).text():'Slide ' + (index + 1);
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

            $(this._node).removeClass(this._states.join(" ") + " noanim");
            
            
            $(this._node).addClass(state);
            var flyback = state != 'distant-slide'
                          && this._currentState != 'distant-slide'
                          && this._states.indexOf(state) > this._states.indexOf(this._currentState);
            if (flyback) {
                //Avoid fly-back when slide count < 7
                $(this._node).addClass('noanim');
            }

            this._currentState = state;

            if (state == 'current') {
              $('#presentation-counter').html(this._title);
              this._onLoad();
            } else {
              (this._titleElem) && (this._titleElem.css('display','none'));
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
                this._titleElem.css('display','block');                
             }
          }
        };

        //
        // SlideShow class
        //
        SlideShow = function() {
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
            if (!_t._isRunning) {
                return;
            }
            _t.next();
            setTimeout(goNext,_t.timeout);
          }
          setTimeout(goNext,_t.timeout);
        };

        SlideShow.prototype = {
          _presentationCounter: $('#presentation-counter'),
          _slides: [],
          _isRunning: true,
          
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
          },
          stop:function() {
            this._isRunning = false;
          }
        };
                
        var startSlideShow = function () {
            slideshow = new SlideShow();
            $('.slides').css('display', 'block');
        };
        
        var stopSlideShow = function () {
            if (typeof(slideshow) != "undefined" && slideshow != undefined) {
               slideshow.stop();
               $('.slides').css('display', 'none');
            }
        };
        
        SlideDescr=function(){};
        SlideDescr.prototype = {
            id: null,
            title: null,
            doc: null,
            image: null,
            generateHtml: function() {
                var html = '<div class="slide" id="' + this.id + '">';
                if (this.title != null && this.title.length > 0) {
                    html += '<section class="title">';
                    html += this.title;
                    html += '</section>';
                }
                if (this.doc != null) {
                    html += '<section class="middle">';
                    html += '<iframe width="100%" height="100%" _src="' + this.doc + '"/>';
                    html += '</section>';                
                } else if (this.image != null) {
                    html += '<section class="middle">';
                    html += '<img style="max-width:95%;max-height:95%" src="' + this.image + '"/>';
                    html += '</section>';
                }
                html += '</div>';
                return html;
            }
        };
        
        var convertFeed = function(xml) {
            var slideDescrs=new Array();
            $('item',xml).each(function(index,item){
                var slide = new SlideDescr();
                slide.id ='slide' + index;
                slide.title = $(this).find('title').eq(0).text();
                slide.doc = $(this).find('content[medium="document"]').eq(0).attr('url');
                slide.image = $(this).find('content[medium="image"]').eq(0).attr('url');
                slideDescrs.push(slide);
            });
            //shuffle slides
            for(var i = slideDescrs.length - 1; i>0;i--) {
                var j = Math.floor(Math.random()*(i+1));
                var tmp = slideDescrs[j];
                slideDescrs[j] = slideDescrs[i];
                slideDescrs[i] = tmp;
            }
            
            var slidesContent = '';
            while(s=slideDescrs.shift()) {
                slidesContent += s.generateHtml();
            }
            return slidesContent;
        };
        
        var loadFeed = function() {
            var feedUrl = $('.slides').attr('feed-url');
            if (feedUrl == undefined) {
                return;
            }
            $.ajax({
               url: feedUrl,
               dataType: 'xml',
               success:function(xml) {
                  stopSlideShow();
                  var html = convertFeed(xml);
                  var feedUpdateTimeout=jQuery('ttl',xml).text()*60*1000;
                  if (isNaN(feedUpdateTimeout) || feedUpdateTimeout < 30000) {
                    feedUpdateTimeout = 10*60*1000;
                  }
                  setTimeout(loadFeed,feedUpdateTimeout);
                  $('.slides').eq(0).html(html);
                  startSlideShow();
               }
            });
        };
        loadFeed();
    
})
