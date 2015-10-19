// Full list of configuration options available here:
// https://github.com/hakimel/reveal.js#configuration
Reveal.initialize({
    controls: true,
    progress: true,
    history: true,
    center: true,
    slideNumber: true,
    transition: 'slide', // none/fade/slide/convex/concave/zoom
    // Optional reveal.js plugins
    dependencies: [
	{ src: '../../slides/reveal.js/lib/js/classList.js', condition: function() { return !document.body.classList; } },
	{ src: '../../slides/reveal.js/plugin/markdown/marked.js', condition: function() { return !!document.querySelector( '[data-markdown]' ); } },
	{ src: '../../slides/reveal.js/plugin/markdown/markdown.js', condition: function() { return !!document.querySelector( '[data-markdown]' ); } },
	{ src: '../../slides/reveal.js/plugin/highlight/highlight.js', async: true, condition: function() { return !!document.querySelector( 'pre code' ); }, callback: function() { hljs.initHighlightingOnLoad(); } },
	{ src: '../../slides/reveal.js/plugin/zoom-js/zoom.js', async: true },
	{ src: '../../slides/reveal.js/plugin/notes/notes.js', async: true }
    ]
});
