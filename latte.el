;;; latte.el --- Major mode for editing Latte source.

;; Copyright 1998,199 Zanshin Inc.           <http://www.zanshin.com/>

;; The contents of this file are subject to the Zanshin Public License Version
;; 1.0 (the "License"); you may not use this file except in compliance with the
;; License.  You should have received a copy of the License with Latte; see
;; the file COPYING.  You may also obtain a copy of the License at
;; <http://www.zanshin.com/ZPL.html>.
;; 
;; Software distributed under the License is distributed on an "AS IS" basis,
;; WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
;; for the specific language governing rights and limitations under the
;; License.
;; 
;; The Original Code is Latte.
;; 
;; The Initial Developer of the Original Code is Zanshin, Inc.

;;; Author:

;; Bob Glickstein <bobg@zanshin.com> <http://www.zanshin.com/~bobg>

;;; Plug:

;; Check out my book, "Writing GNU Emacs Extensions," from O'Reilly
;; and Associates.  <http://www.ora.com/catalog/gnuext/>

;;; Code:

(require 'derived)

(defvar latte-mode-map nil
  "*Keymap for Latte mode.")
(if latte-mode-map
    nil
  (setq latte-mode-map (copy-keymap text-mode-map))
  (substitute-key-definition 'backward-delete-char
                             'backward-delete-char-untabify
                             latte-mode-map)
  (substitute-key-definition 'delete-backward-char
                             'backward-delete-char-untabify
                             latte-mode-map)
  (define-key latte-mode-map "\t" 'indent-for-tab-command)
  )

(defvar latte-mode-syntax-table nil
  "*Syntax table for Latte mode.")
(if latte-mode-syntax-table
    nil
  (setq latte-mode-syntax-table (copy-syntax-table text-mode-syntax-table))
  (modify-syntax-entry ?\\ "\\" latte-mode-syntax-table))

(defvar latte-font-lock-keywords
  '(("\\\\;.*" . font-lock-comment-face)
    ("\\\\\\\\" . font-lock-keyword-face)
    ("\\\\/" . font-lock-keyword-face)
    ("\\\\['`]" . font-lock-keyword-face)
    ("\\\\,@?" . font-lock-keyword-face)
    ;; The following regex was created with
    ;; (sregexq "\\\""
    ;;          (0+ (or (not-char "\\")
    ;;                  (sequence "\\" (not-char "\""))))
    ;;          "\\\"")
    ;; See <http://www.zanshin.com/~bobg/sregex.html>
    ("\\\\\"\\([^\\\\]\\|\\\\[^\"]\\)*\\\\\"" 0 font-lock-string-face t)
    ("{\\(\\\\\\(def\\|if\\|lambda\\)\\)\\>" 1 font-lock-keyword-face)
    ("{\\(\\\\[A-Za-z_][A-Za-z_0-9?!+-]*\\)" 1 font-lock-function-name-face)
    ("\\\\[&=]?[A-Za-z_][A-Za-z_0-9?!+-]*" . font-lock-variable-name-face))
  "*Value of font-lock-keywords for Latte mode.")

(define-derived-mode latte-mode text-mode "Latte"
  "Major mode for editing Latte source."
  (make-local-variable 'comment-column)
  (make-local-variable 'comment-start-skip)
  (make-local-variable 'comment-start)
  (make-local-variable 'comment-end)
  (make-local-variable 'font-lock-defaults)
  (make-local-variable 'indent-line-function)
  (make-local-variable 'imenu-generic-expression)
  (setq comment-column 40
        comment-start-skip "\\\\;\\s-*"
        comment-start "\\; "
        comment-end ""                  ;xxx also set comment-indent-function?
        font-lock-defaults '(latte-font-lock-keywords t)
        indent-line-function 'latte-indent-line
        )
  (setq imenu-generic-expression
        '((nil "^{\\\\def\\s-*\\\\\\([A-Za-z_][A-Za-z_0-9?!+-]*\\)" 1))))

(defconst latte-keyword-regex
  (concat "\\\\\\("
          (mapconcat 'symbol-name
                     '(def if let while)
                     "\\|")
          "\\)\\>"))

(defun latte-indent-line ()
  (save-match-data
    (let* ((start (point))

           ;; the position of the first non-whitespace on the line
           (bol (progn (back-to-indentation)
                       (point)))

           ;; the position of the innermost open-brace enclosing bol
           (open (condition-case nil
                     (let ((looping t))
                       (while looping
                         (backward-up-list 1)
                         (setq looping
                               (not (= (following-char) ?{))))
                       (point))
                   (error nil)))

           ;; the column of that open-brace
           (open-column (and open (current-column)))

           ;; whether that open-brace is followed by one of a few
           ;; keywords with special indentation rules
           (keyword (and open
                         (progn
                           (forward-char 1)
                           (and (looking-at latte-keyword-regex)
                                (match-string 1)))))

           ;; the position of the first non-whitespace on the previous
           ;; nonblank line
           (prev-bol (progn
                       (goto-char bol)
                       (and (zerop (forward-line -1))
                            (progn
                              (end-of-line)
                              (skip-chars-backward " \t\n\r\f")
                              (and (not (memq (preceding-char)
                                              '(0 ?\  ?\t ?\n ?\r ?\f)))
                                   (progn
                                     (back-to-indentation)
                                     (point)))))))

           ;; the indentation of the previous nonblank line relative
           ;; to start
           (prev-indent (and prev-bol
                             (current-indentation)))

           ;; the position of the open-brace enclosing the beginning
           ;; of the previous line
           (prev-open (and prev-bol
                           (progn
                             (condition-case nil
                                 (let ((looping t))
                                   (while looping
                                     (backward-up-list 1)
                                     (setq looping
                                           (not (= (following-char) ?{))))
                                   (point))
                               (error nil)))))

           ;; the result
           (column (cond (keyword (+ open-column 2))

                         ;; if current line and previous line are
                         ;; contained in the same level of braces, use
                         ;; previous line's indentation
                         ((and prev-open
                               (equal open prev-open))
                          prev-indent)

                         ;; if current line is enclosed in braces, use
                         ;; the brace's column + 1
                         (open (1+ open-column))

                         ;; Use previous line's indentation
                         (prev-indent prev-indent)

                         ;; No indentation
                         (t 0))))
      (goto-char bol)
      (if (and (= start bol)
               (>= (current-indentation) column))
          ;; repeated presses of TAB should indent the line further
          (indent-relative)
        (indent-line-to column)))))

;;; latte.el ends here
