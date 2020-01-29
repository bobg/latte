# Copyright 1998 Zanshin Inc.

# The contents of this file are subject to the Zanshin Public License
# Version 1.0 (the "License"); you may not use this file except in
# compliance with the License.  You should have received a copy of the
# License with Latte; see the file COPYING.  You may also obtain a
# copy of the License at <http://www.zanshin.com/ZPL.html>.
# 
# Software distributed under the License is distributed on an "AS IS"
# basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See
# the License for the specific language governing rights and
# limitations under the License.
# 
# The Original Code is Latte.
# 
# The Initial Developer of the Original Code is Zanshin, Inc.

package Latteizer;

use HTML::Parser;
use HTML::Entities;

@ISA = qw(HTML::Parser);

%tags = ('a' => { 'params' => [qw(accesskey charset class coords dir
                                  href hreflang id lang name onblur
                                  onclick ondblclick onfocus onkeydown
                                  onkeypress onkeyup onmousedown
                                  onmousemove onmouseout onmouseover
                                  onmouseup rel rev shape style
                                  tabindex target title type)] },
         'abbr' => { 'params' => [qw(class dir id lang onclick
                                     ondblclick onkeydown onkeypress
                                     onkeyup onmousedown onmousemove
                                     onmouseout onmouseover onmouseup
                                     style title)] },
         'acronym' => { 'params' => [qw(class dir id lang onclick
                                        ondblclick onkeydown
                                        onkeypress onkeyup onmousedown
                                        onmousemove onmouseout
                                        onmouseover onmouseup style
                                        title)] },
         'address' => { 'params' => [qw(class dir id lang onclick
                                        ondblclick onkeydown
                                        onkeypress onkeyup onmousedown
                                        onmousemove onmouseout
                                        onmouseover onmouseup style
                                        title)] },
         'b' => { 'params' => [qw(class  dir  id  lang  onclick 
                                  ondblclick  onkeydown  onkeypress
                                  onkeyup  onmousedown 
                                  onmousemove  onmouseout  onmouseover
                                  onmouseup  style  title
                                  )] },
         'bdo' => { 'params' => [qw(class  dir  id  lang  style  title
                                    )] },
         'big' => { 'params' => [qw(class  dir  id  lang  onclick 
                                    ondblclick  onkeydown  onkeypress
                                    onkeyup  onmousedown 
                                    onmousemove  onmouseout  onmouseover
                                    onmouseup  style  title
                                    )] },
         'blockquote' => { 'params' => [qw(cite  class  dir  id  lang  onclick
                                           ondblclick  onkeydown 
                                           onkeypress  onkeyup  onmousedown
                                           onmousemove  onmouseout
                                           onmouseover  onmouseup 
                                           style  title)] },
         'body' => { 'params' => [qw(class  dir  id  lang  onclick 
                                     ondblclick  onkeydown  onkeypress
                                     onkeyup  onload  onmousedown
                                     onmousemove  onmouseout
                                     onmouseover  onmouseup 
                                     onunload  style  title alink  background  bgcolor  link
                                     text  vlink )] },
         'button' => { 'params' => [qw(accesskey class dir id lang
                                       name onblur onclick ondblclick
                                       onfocus onkeydown onkeypress
                                       onkeyup onmousedown onmousemove
                                       onmouseout onmouseover
                                       onmouseup style tabindex title
                                       type value)],
                                           'bools' => [qw(disabled)] },
         'caption' => { 'params' => [qw(class  dir  id  lang  onclick 
                                        ondblclick  onkeydown  onkeypress
                                        onkeyup  onmousedown 
                                        onmousemove  onmouseout  onmouseover
                                        onmouseup  style  title
                                        )],
                                            'bools' => [qw(align)] },
         'cite' => { 'params' => [qw(class  dir  id  lang  onclick 
                                     ondblclick  onkeydown  onkeypress
                                     onkeyup  onmousedown 
                                     onmousemove  onmouseout  onmouseover
                                     onmouseup  style  title
                                     )] },
         'code' => { 'params' => [qw(class  dir  id  lang  onclick 
                                     ondblclick  onkeydown  onkeypress
                                     onkeyup  onmousedown 
                                     onmousemove  onmouseout  onmouseover
                                     onmouseup  style  title
                                     )] },
         'colgroup' => { 'unwind' => [qw(table)],
                         'params' => [qw(align  char  charoff  class  dir
                                         id  lang  onclick  ondblclick
                                         onkeydown  onkeypress 
                                         onkeyup  onmousedown  onmousemove
                                         onmouseout  onmouseover
                                         onmouseup  span  style 
                                         title  valign  width)] },
         'dd' => { 'unwind' => [qw(dl)],
                   'params' => [qw(class  dir  id  lang  onclick 
                                   ondblclick  onkeydown  onkeypress
                                   onkeyup  onmousedown 
                                   onmousemove  onmouseout  onmouseover
                                   onmouseup  style  title
                                   )] },
         'del' => { 'params' => [qw(cite  class  datetime  dir  id 
                                    lang  onclick  ondblclick 
                                    onkeydown  onkeypress  onkeyup
                                    onmousedown  onmousemove 
                                    onmouseout  onmouseover  onmouseup
                                    style  title)] },
         'dfn' => { 'params' => [qw(class  dir  id  lang  onclick 
                                    ondblclick  onkeydown  onkeypress
                                    onkeyup  onmousedown 
                                    onmousemove  onmouseout  onmouseover
                                    onmouseup  style  title
                                    )] },
         'div' => { 'params' => [qw(class  dir  id  lang  onclick 
                                    ondblclick  onkeydown  onkeypress
                                    onkeyup  onmousedown 
                                    onmousemove  onmouseout  onmouseover
                                    onmouseup  style  title align
                                    )] },
         'dl' => { 'params' => [qw(class  dir  id  lang  onclick 
                                   ondblclick  onkeydown  onkeypress
                                   onkeyup  onmousedown 
                                   onmousemove  onmouseout  onmouseover
                                   onmouseup  style  title
                                   )] },
         'dt' => { 'unwind' => [qw(dl)],
                   'params' => [qw(class  dir  id  lang  onclick 
                                   ondblclick  onkeydown  onkeypress
                                   onkeyup  onmousedown 
                                   onmousemove  onmouseout  onmouseover
                                   onmouseup  style  title
                                   )] },
         'em' => { 'params' => [qw(class  dir  id  lang  onclick 
                                   ondblclick  onkeydown  onkeypress
                                   onkeyup  onmousedown 
                                   onmousemove  onmouseout  onmouseover
                                   onmouseup  style  title
                                   )] },
         'fieldset' => { 'params' => [qw(class  dir  id  lang  onclick 
                                         ondblclick  onkeydown  onkeypress
                                         onkeyup  onmousedown 
                                         onmousemove  onmouseout  onmouseover
                                         onmouseup  style  title
                                         )] },
         'form' => { 'params' => [qw(accept-charset -charset action  class 
                                     dir  enctype  id  lang  method 
                                     onclick  ondblclick  onkeydown
                                     onkeypress  onkeyup 
                                     onmousedown  onmousemove 
                                     onmouseout  onmouseover  onmouseup
                                     onreset  onsubmit  style 
                                     target  title)] },
         'frameset' => { 'params' => [qw(class  cols  id  onload  onunload
                                         rows  style  title)] },
         'h1' => { 'params' => [qw(class  dir  id  lang  onclick 
                                   ondblclick  onkeydown  onkeypress
                                   onkeyup  onmousedown 
                                   onmousemove  onmouseout  onmouseover
                                   onmouseup  style  title align
                                   )] },
         'h2' => { 'params' => [qw(class  dir  id  lang  onclick 
                                   ondblclick  onkeydown  onkeypress
                                   onkeyup  onmousedown 
                                   onmousemove  onmouseout  onmouseover
                                   onmouseup  style  title align
                                   )] },
         'h3' => { 'params' => [qw(class  dir  id  lang  onclick 
                                   ondblclick  onkeydown  onkeypress
                                   onkeyup  onmousedown 
                                   onmousemove  onmouseout  onmouseover
                                   onmouseup  style  title align
                                   )] },
         'h4' => { 'params' => [qw(class  dir  id  lang  onclick 
                                   ondblclick  onkeydown  onkeypress
                                   onkeyup  onmousedown 
                                   onmousemove  onmouseout  onmouseover
                                   onmouseup  style  title align
                                   )] },
         'h5' => { 'params' => [qw(class  dir  id  lang  onclick 
                                   ondblclick  onkeydown  onkeypress
                                   onkeyup  onmousedown 
                                   onmousemove  onmouseout  onmouseover
                                   onmouseup  style  title align
                                   )] },
         'h6' => { 'params' => [qw(class  dir  id  lang  onclick 
                                   ondblclick  onkeydown  onkeypress
                                   onkeyup  onmousedown 
                                   onmousemove  onmouseout  onmouseover
                                   onmouseup  style  title align
                                   )] },
         'head' => { 'params' => [qw(dir  lang  profile)] },
         'i' => { 'params' => [qw(class  dir  id  lang  onclick 
                                  ondblclick  onkeydown  onkeypress
                                  onkeyup  onmousedown 
                                  onmousemove  onmouseout  onmouseover
                                  onmouseup  style  title
                                  )] },
         'iframe' => { 'params' => [qw(align  class  frameborder  height
                                       id  longdesc  marginheight
                                       marginwidth  name  scrolling
                                       src  style  target  title
                                       width)] },
         'ins' => { 'params' => [qw(cite  class  datetime  dir  id 
                                    lang  onclick  ondblclick 
                                    onkeydown  onkeypress  onkeyup
                                    onmousedown  onmousemove 
                                    onmouseout  onmouseover  onmouseup
                                    style  title)] },
         'kbd' => { 'params' => [qw(class  dir  id  lang  onclick 
                                    ondblclick  onkeydown  onkeypress
                                    onkeyup  onmousedown 
                                    onmousemove  onmouseout  onmouseover
                                    onmouseup  style  title
                                    )] },
         'label' => { 'params' => [qw(accesskey  class  dir  for  id 
                                      lang  onblur  onclick  ondblclick
                                      onfocus  onkeydown 
                                      onkeypress  onkeyup  onmousedown
                                      onmousemove  onmouseout
                                      onmouseover  onmouseup 
                                      style  title)] },
         'legend' => { 'params' => [qw(accesskey  class  dir  id  lang
                                       onclick  ondblclick  onkeydown
                                       onkeypress  onkeyup 
                                       onmousedown  onmousemove 
                                       onmouseout  onmouseover  onmouseup
                                       style  title align)] },
         'li' => { 'unwind' => [qw(ul ol)],
                   'params' => [qw(class  dir  id  lang  onclick 
                                   ondblclick  onkeydown  onkeypress
                                   onkeyup  onmousedown 
                                   onmousemove  onmouseout  onmouseover
                                   onmouseup  style  title
                                   compact type value
                                   )] },
         'map' => { 'params' => [qw(class  dir  id  lang  name  onclick
                                    ondblclick  onkeydown 
                                    onkeypress  onkeyup  onmousedown
                                    onmousemove  onmouseout
                                    onmouseover  onmouseup 
                                    style  title)] },
         'noframes' => { 'params' => [qw(class  dir  id  lang  onclick 
                                         ondblclick  onkeydown  onkeypress
                                         onkeyup  onmousedown 
                                         onmousemove  onmouseout  onmouseover
                                         onmouseup  style  title
                                         )] },
         'noscript' => { 'params' => [qw(class  dir  id  lang  onclick 
                                         ondblclick  onkeydown  onkeypress
                                         onkeyup  onmousedown 
                                         onmousemove  onmouseout  onmouseover
                                         onmouseup  style  title
                                         )] },
         'object' => { 'params' => [qw(archive  class  classid  codebase
                                       codetype  data 
                                       dir  id  lang  name  onclick 
                                       ondblclick  onkeydown  onkeypress
                                       onkeyup  onmousedown 
                                       onmousemove  onmouseout  onmouseover
                                       onmouseup  standby  style
                                       tabindex  title  type  usemap
                                       align  border  height  hspace 
                                       vspace  width
                                       )],
                                           'bools' => [qw(declare)] },
         'ol' => { 'params' => [qw(class  dir  id  lang  onclick 
                                   ondblclick  onkeydown  onkeypress
                                   onkeyup  onmousedown 
                                   onmousemove  onmouseout  onmouseover
                                   onmouseup  style  title
                                   compact start type
                                   )] },
         'optgroup' => { 'params' => [qw(class  dir  id  label  lang 
                                         onclick  ondblclick  onkeydown
                                         onkeypress  onkeyup 
                                         onmousedown  onmousemove 
                                         onmouseout  onmouseover  onmouseup
                                         style  title)],
                                             'bools' => [qw(disabled)] },
         'option' => { 'unwind' => [qw(select optgroup)],
                       'params' => [qw(class  dir  id  label  lang 
                                       onclick  ondblclick  onkeydown
                                       onkeypress  onkeyup 
                                       onmousedown  onmousemove 
                                       onmouseout  onmouseover  onmouseup
                                       style  title  value)],
                                           'bools' => [qw(disabled selected)] },
         'pre' => { 'params' => [qw(class  dir  id  lang  onclick 
                                    ondblclick  onkeydown  onkeypress
                                    onkeyup  onmousedown 
                                    onmousemove  onmouseout  onmouseover
                                    onmouseup  style  title width
                                    )] },
         'q' => { 'params' => [qw(cite  class  dir  id  lang  onclick
                                  ondblclick  onkeydown 
                                  onkeypress  onkeyup  onmousedown
                                  onmousemove  onmouseout
                                  onmouseover  onmouseup 
                                  style  title)] },
         'samp' => { 'params' => [qw(class  dir  id  lang  onclick 
                                     ondblclick  onkeydown  onkeypress
                                     onkeyup  onmousedown 
                                     onmousemove  onmouseout  onmouseover
                                     onmouseup  style  title
                                     )] },
         'script' => { 'params' => [qw(charset  src  type
                                       language)],
                           'bools' => [qw(defer)] },
         'select' => { 'params' => [qw(class  dir  id  lang  name  onblur
                                       onchange  onclick  ondblclick
                                       onfocus  onkeydown 
                                       onkeypress  onkeyup  onmousedown
                                       onmousemove  onmouseout
                                       onmouseover  onmouseup 
                                       size  style  tabindex  title)],
                                           'bools' => [qw(disabled multiple)] },
         'small' => { 'params' => [qw(class  dir  id  lang  onclick 
                                      ondblclick  onkeydown  onkeypress
                                      onkeyup  onmousedown 
                                      onmousemove  onmouseout  onmouseover
                                      onmouseup  style  title
                                      )] },
         'span' => { 'params' => [qw(class  dir  id  lang  onclick 
                                     ondblclick  onkeydown  onkeypress
                                     onkeyup  onmousedown 
                                     onmousemove  onmouseout  onmouseover
                                     onmouseup  style  title align
                                     )] },
         'strong' => { 'params' => [qw(class  dir  id  lang  onclick 
                                       ondblclick  onkeydown  onkeypress
                                       onkeyup  onmousedown 
                                       onmousemove  onmouseout  onmouseover
                                       onmouseup  style  title
                                       )] },
         'style' => { 'params' => [qw(dir  lang  media  title  type)] },
         'sub' => { 'params' => [qw(class  dir  id  lang  onclick 
                                    ondblclick  onkeydown  onkeypress
                                    onkeyup  onmousedown 
                                    onmousemove  onmouseout  onmouseover
                                    onmouseup  style  title
                                    )] },
         'sup' => { 'params' => [qw(class  dir  id  lang  onclick 
                                    ondblclick  onkeydown  onkeypress
                                    onkeyup  onmousedown 
                                    onmousemove  onmouseout  onmouseover
                                    onmouseup  style  title
                                    )] },
         'table' => { 'params' => [qw(border  cellpadding  cellspacing
                                      class  dir  frame  id  lang
                                      onclick  ondblclick  onkeydown
                                      onkeypress  onkeyup 
                                      onmousedown  onmousemove 
                                      onmouseout  onmouseover  onmouseup
                                      rules  style  summary  title
                                      width align bgcolor)] },
         'tbody' => { 'unwind' => [qw(table)],
                      'params' => [qw(align  char  charoff  class  dir
                                      id  lang  onclick  ondblclick
                                      onkeydown  onkeypress 
                                      onkeyup  onmousedown  onmousemove
                                      onmouseout  onmouseover
                                      onmouseup  style  title 
                                      valign)] },
         'td' => { 'unwind' => [qw(tr)],
                   'params' => [qw(abbr  align  axis  char  charoff
                                   class  colspan  dir  headers
                                   id  lang  onclick  ondblclick
                                   onkeydown  onkeypress 
                                   onkeyup  onmousedown  onmousemove
                                   onmouseout  onmouseover
                                   onmouseup  rowspan  scope
                                   style  title  valign bgcolor height width)],
                                       'bools' => [qw(nowrap)] },
         'textarea' => { 'params' => [qw(accesskey  class  cols  dir  id
                                         lang  name  onblur  onchange 
                                         onclick  ondblclick  onfocus 
                                         onkeydown  onkeypress  onkeyup
                                         onmousedown  onmousemove 
                                         onmouseout  onmouseover  onmouseup
                                         onselect  rows  style 
                                         tabindex  title)],
                                             'bools' => [qw(disabled readonly)] },
         'tfoot' => { 'unwind' => [qw(table)],
                      'params' => [qw(align  char  charoff  class  dir
                                      id  lang  onclick  ondblclick
                                      onkeydown  onkeypress 
                                      onkeyup  onmousedown  onmousemove
                                      onmouseout  onmouseover
                                      onmouseup  style  title 
                                      valign)] },
         'th' => { 'unwind' => [qw(tr)],
                   'params' => [qw(abbr  align  axis  char  charoff
                                   class  colspan  dir  headers
                                   id  lang  onclick  ondblclick
                                   onkeydown  onkeypress 
                                   onkeyup  onmousedown  onmousemove
                                   onmouseout  onmouseover
                                   onmouseup  rowspan  scope
                                   style  title  valign bgcolor height width)],
                                       'bools' => [qw(nowrap)] },
         'thead' => { 'unwind' => [qw(table)],
                      'params' => [qw(align  char  charoff  class  dir
                                      id  lang  onclick  ondblclick
                                      onkeydown  onkeypress 
                                      onkeyup  onmousedown  onmousemove
                                      onmouseout  onmouseover
                                      onmouseup  style  title 
                                      valign)] },
         'title' => { 'params' => [qw(dir  lang)] },
         'tr' => { 'unwind' => [qw(table)],
                   'params' => [qw(align  char  charoff  class  dir
                                   id  lang  onclick  ondblclick
                                   onkeydown  onkeypress 
                                   onkeyup  onmousedown  onmousemove
                                   onmouseout  onmouseover
                                   onmouseup  style  title 
                                   valign bgcolor)] },
         'tt' => { 'params' => [qw(class  dir  id  lang  onclick 
                                   ondblclick  onkeydown  onkeypress
                                   onkeyup  onmousedown 
                                   onmousemove  onmouseout  onmouseover
                                   onmouseup  style  title
                                   )] },
         'ul' => { 'params' => [qw(class  dir  id  lang  onclick 
                                   ondblclick  onkeydown  onkeypress
                                   onkeyup  onmousedown 
                                   onmousemove  onmouseout  onmouseover
                                   onmouseup  style  title compact type
                                   )] },
         'var' => { 'params' => [qw(class  dir  id  lang  onclick 
                                    ondblclick  onkeydown  onkeypress
                                    onkeyup  onmousedown 
                                    onmousemove  onmouseout  onmouseover
                                    onmouseup  style  title
                                    )] },
         'applet' => { 'params' => [qw(align  alt  archive  class  code
                                       codebase  height  hspace  id
                                       name  object  style  title 
                                       vspace  width)] },
         'center' => { },
         'dir' => { 'params' => [qw(class  dir  id  lang  onclick 
                                    ondblclick  onkeydown  onkeypress
                                    onkeyup  onmousedown 
                                    onmousemove  onmouseout  onmouseover
                                    onmouseup  style  title
                                    )] },
         'font' => { 'params' => [qw(class  color  dir  face  id  lang
                                     size  style  title)] },
         'menu' => { 'params' => [qw(class  dir  id  lang  onclick 
                                     ondblclick  onkeydown  onkeypress
                                     onkeyup  onmousedown 
                                     onmousemove  onmouseout  onmouseover
                                     onmouseup  style  title
                                     )] },
         's' => { 'params' => [qw(class  dir  id  lang  onclick 
                                  ondblclick  onkeydown  onkeypress
                                  onkeyup  onmousedown 
                                  onmousemove  onmouseout  onmouseover
                                  onmouseup  style  title
                                  )] },
         'strike' => { 'params' => [qw(class  dir  id  lang  onclick 
                                       ondblclick  onkeydown  onkeypress
                                       onkeyup  onmousedown 
                                       onmousemove  onmouseout  onmouseover
                                       onmouseup  style  title
                                       )] },
         'u' => { 'params' => [qw(class  dir  id  lang  onclick 
                                  ondblclick  onkeydown  onkeypress
                                  onkeyup  onmousedown 
                                  onmousemove  onmouseout  onmouseover
                                  onmouseup  style  title)] },
         'p' => { 'unbalanced' => 1,
                  'params' => [qw(class  dir  id  lang  onclick 
                                  ondblclick  onkeydown  onkeypress
                                  onkeyup  onmousedown 
                                  onmousemove  onmouseout  onmouseover
                                  onmouseup  style  title align)] },
         'area' => { 'unbalanced' => 1,
                     'params' => [qw(accesskey  alt  class  coords  dir
                                     href  id  lang  name 
                                     onblur  onclick  ondblclick  onfocus
                                     onkeydown  onkeypress  onkeyup
                                     onmousedown  onmousemove 
                                     onmouseout  onmouseover  onmouseup
                                     shape  style  tabindex  target
                                     title  usemap)],
                                         'bools' => [qw(nohref)] },
         'base' => { 'unbalanced' => 1,
                     'params' => [qw(href  target)] },
         'br' => { 'unbalanced' => 1,
                   'params' => [qw(class  id  style  title clear)] },
         'col' => { 'unbalanced' => 1,
                    'params' => [qw(align  char  charoff  class  dir 
                                    id  lang  onclick  ondblclick 
                                    onkeydown  onkeypress  onkeyup 
                                    onmousedown  onmousemove  onmouseout
                                    onmouseover  onmouseup  span
                                    style  title  valign  width)] },
         'frame' => { 'unbalanced' => 1,
                      'params' => [qw(class  frameborder  id  longdesc 
                                      marginheight  marginwidth  name 
                                      scrolling  src  style 
                                      target  title)],
                                          'bools' => [qw(noresize)] },
         'hr' => { 'unbalanced' => 1,
                   'params' => [qw(class  id  onclick  ondblclick 
                                   onkeydown  onkeypress  onkeyup 
                                   onmousedown  onmousemove  onmouseout
                                   onmouseover  onmouseup  style
                                   title align size width)],
                                       'bools' => [qw(noshade)] },
         'img' => { 'unbalanced' => 1,
                    'params' => [qw(align  alt  class  dir  height
                                    hspace  id  lang 
                                    longdesc  onclick  ondblclick
                                    onkeydown  onkeypress 
                                    onkeyup  onmousedown  onmousemove
                                    onmouseout  onmouseover
                                    onmouseup  src  style 
                                    title  usemap  vspace  width border)],
                                        'bools' => [qw(ismap)] },
         'input' => { 'unbalanced' => 1,
                      'params' => [qw(accept  accesskey  alt  class  dir
                                      id  lang  maxlength  name  onblur
                                      onchange  onclick  ondblclick
                                      onfocus  onkeydown  onkeypress
                                      onkeyup  onmousedown 
                                      onmousemove  onmouseout  onmouseover
                                      onmouseup  onselect  size 
                                      src  style  tabindex  title  type
                                      usemap  value)],
                                          'bools' => [qw(checked disabled readonly)] },
         'link' => { 'unbalanced' => 1,
                     'params' => [qw(charset  class  dir  href  hreflang
                                     id  lang  media  onclick 
                                     ondblclick  onkeydown  onkeypress
                                     onkeyup  onmousedown 
                                     onmousemove  onmouseout  onmouseover
                                     onmouseup  rel  rev  style
                                     target  title  type)] },
         'meta' => { 'unbalanced' => 1,
                     'params' => [qw(content  dir  http-equiv -equiv lang 
                                     name  scheme)] },
         'param' => { 'unbalanced' => 1,
                      'params' => [qw(id  name  type  value  valuetype)] },
         'basefont' => { 'unbalanced' => 1,
                         'params' => [qw(class  color  dir  face  id  lang 
                                         size  style  title)] },
         'isindex' => { 'unbalanced' => 1,
                        'params' => [qw(class  dir  id  lang  prompt  style
                                        title)] });


@unbalanced = qw(p area base br col frame hr img input link meta
                 param basefont isindex);               

@stack = ();

sub new {
  my($type) = shift;
  my($self) = new HTML::Parser();

  bless $self, $type;
}

sub declaration {
  my($self, $decl) = @_;

  # silently discard
}

sub start {
  my($self, $tag, $attr, $attrseq, $origtext) = @_;

  return if ($tag eq 'html');

  if (($tag eq 'p') && !@$attrseq) {
    print "\n\n";
    return;
  }

  my($hash) = $tags{$tag};

  die "Unknown HTML element $tag\n" unless $hash;

  my($params, $bools, $unbalanced, $unwind) =
      ($hash->{'params'}, $hash->{'bools'},
       $hash->{'unbalanced'}, $hash->{'unwind'});

  if ($unwind) {
    while (1) {
      die "Element $tag unwound the stack too far\n" unless @stack;
      last if (grep { $_ eq $stack[$#stack] } @$unwind);
      $self->end($stack[$#stack]);
    }
  }

  print "{\\$tag";

  my(@nonstandard) = ();

  foreach my $name (@$attrseq) {
    my($val) = $attr->{$name};

    $val = &decode_entities($val);

    $val =~ s/&([a-zA-Z]+);/{\\ch $1}/g;
    $val =~ s/([\\{}])/\\$1/g;

    if ($params && (grep { $_ eq $name } @$params)) {
      print " \\$name={$val}";
    } elsif ($bools && (grep { $_ eq $name } @$bools)) {
      print " \\$name=1";
    } else {
      push(@nonstandard, [$name, $val]);
    }
  }

  if (@nonstandard) {
    print " \\nonstandard={";
    foreach my $pair (@nonstandard) {
      print sprintf('{%s} {%s} ', @$pair);
    }
    print "}";
  }

  if ($unbalanced) {
    print "}";
  } else {
    print ' ';
    push(@stack, $tag);
  }
}

sub end {
  my($self, $tag, $origtext) = @_;

  my($hash) = $tags{$tag};

  return if ($hash && $hash->{'unbalanced'});
  return if ($tag eq 'html');

  while (1) {
    die "Stack underflow\n" unless @stack;
    print "}";
    my($top) = pop(@stack);
    last if ($top eq $tag);
  }
}

sub text {
  my($self, $text) = @_;

  $text = &decode_entities($text);

  $text =~ s/&([a-zA-Z]+);/{\\ch $1}/g;
  $text =~ s/([\\{}])/\\$1/g;

  print $text;
}

package main;

$l = new Latteizer;
$l->parse_file(shift);
