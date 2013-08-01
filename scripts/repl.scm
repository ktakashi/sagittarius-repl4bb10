;; NOTE: GUI side will delete the mark file before
;; executing the server process.

(import (rnrs) (sagittarius remote-repl) (sagittarius))
(define server-log (build-path (getenv "HOME") "repl.log"))
(when (file-exists? server-log)
  (delete-file server-log))
(define log-port (open-file-output-port server-log
					(file-options no-fail)
					(buffer-mode none)
					(native-transcoder)))

(define (main args)
  (define server (make-remote-repl (cadr args) :log log-port))
  ;; when it's ready create a file
  (call-with-output-file (build-path (getenv "HOME") "ready")
    (lambda (o) (put-string o "ready")))
  (server))