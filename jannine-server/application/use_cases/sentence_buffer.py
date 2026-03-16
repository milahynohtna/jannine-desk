import re

class SentenceBuffer:
    """
    Accumulates tokens and flushes complete sentences for TTS generation.
    Doesn't wait for all tokens — flushes eagerly on sentence boundaries.
    """

    SENTENCE_END_PATTERN = re.compile(r'(?<=[.!?])\s+|(?<=[.!?])$')
    MIN_FLUSH_LENGTH = 8  # Minimum chars before flushing to avoid tiny fragments

    def __init__(self):
        self._buffer = ""

    def push(self, token: str) -> list[str]:
        """
        Add a token to the buffer.
        Returns a list of complete sentences ready for TTS (may be empty).
        """
        self._buffer += token
        return self._flush_complete_sentences()

    def flush_remaining(self) -> list[str]:
        """
        Force-flush whatever is left in the buffer (call on DoneMessage).
        """
        remaining = self._buffer.strip()
        self._buffer = ""
        return [remaining] if remaining else []

    def _flush_complete_sentences(self) -> list[str]:
        sentences = []

        while True:
            match = self.SENTENCE_END_PATTERN.search(self._buffer)
            if not match:
                break

            candidate = self._buffer[:match.start() + 1].strip()
            rest = self._buffer[match.end():]

            # Skip flushing if the candidate is too short (e.g. "Dr. " or "U.S.")
            if len(candidate) < self.MIN_FLUSH_LENGTH:
                break

            sentences.append(candidate)
            self._buffer = rest

        return sentences