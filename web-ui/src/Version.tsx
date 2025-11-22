import { useState, useEffect } from "react";
import { setMessageHandler } from "./mqclient";


export default function Version({ }) {
  const [VersionState, setVersionState] = useState({
    major: 0,
    minor: 0,
    patch: 0,
    buildId: 0,
    upTime: 0,
    deviceId: 0,
    project: "",
  });
  useEffect(() => {
    setMessageHandler("Version", setVersionState);
  }, []);

  return (
    <details className="widget">
      <summary>FW Meta</summary>
      <dl className="telemetry">
        <dt>Project</dt>
        <dd data-testid="fw-project">{VersionState.project}</dd>
        <dt>Version</dt>
        <dd data-testid="fw-semver">
          {VersionState.major}.{VersionState.minor}.{VersionState.patch}
        </dd>
        <dt>Build Id</dt>
        <dd data-testid="fw-build-id">{VersionState.buildId}</dd>
        <dt>Device Id</dt>
        <dd data-testid="mcu-id">{VersionState.deviceId}</dd>
        <dt>Up Time</dt>
        <dd data-testid="fw-up-time">{VersionState.upTime}</dd>
      </dl>
    </details>
  );
}