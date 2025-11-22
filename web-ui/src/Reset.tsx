import { sendMessage } from "./mqclient";

export default function Reset({}) {

  function handleSubmit(e: React.FormEvent) {
    e.preventDefault();
    sendMessage("Reset", {});
  }

  return (
    <form className="widget" aria-label="Reset" onSubmit={handleSubmit}>
      <p className="widget-head">
        <span>Reset</span>
        <button type="submit">Send</button>
      </p>
    </form>
  );
}