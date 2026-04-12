markdown:
	@echo Converting markdown files to html format...
	@chmod 755 utils/convert-markdown-to-html
	@utils/convert-markdown-to-html
	@echo done!

touchall:
	find . | grep "\.md$$" | awk '{print "touch "$$1}' | bash

clean:
	/bin/rm -rf *~ */*~ */*/*~
