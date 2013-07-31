;; NOTE: GUI side will delete the mark file before
;; executing the server process.

(import (rnrs) (sagittarius remote-repl) (sagittarius))
(define server (make-remote-repl "12345"))
;; when it's ready create a file
(call-with-output-file (build-path (getenv "HOME") "ready")
  (lambda (o) (put-string o "ready")))
(server)