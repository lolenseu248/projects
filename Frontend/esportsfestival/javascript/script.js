// Select all heading tags (h1 to h6) and paragraphs (p)
const scrollElements = document.querySelectorAll(".ani");

// Check if an element is in the viewport
const isInViewport = (el) => {
  const rect = el.getBoundingClientRect();
  return rect.top < window.innerHeight - 100 && rect.bottom > 0;
};

// Add the 'visible' class when an element enters the viewport
const handleScroll = () => {
  scrollElements.forEach((el) => {
    if (isInViewport(el)) {
      el.classList.add("visible");
    }
  });
};

// Listen for the scroll event
window.addEventListener("scroll", handleScroll);

// Run on page load to catch any already-visible elements
handleScroll();
