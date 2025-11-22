import { useState, useEffect } from "react";
import { selectDevice, setupMq, teardownMq } from './mqclient';

// I need to pass setOnlineDevices to mqclient. 
// But should this happen through setupMq() or like setMessageHandler()?


interface Status { colors: string; text: string };
interface ConnectOptions {
  connected: Status, stale: Status, connecting: Status, disconnected: Status
};
const statuses: ConnectOptions = {
  connected: { colors: "success-colors", text: "Active Data" },
  stale: { colors: "warn-colors", text: "Connected, No data" },
  connecting: { colors: "warn-colors", text: "Connecting" },
  disconnected: { colors: "problem-colors", text: "Not Connected" }
};

export default function BrokerAddress({ showBrokerUrl = false }) {
  const [address, setAddress] = useState("listpalette.com");
  const [selectedDevice, setSelectedDevice] = useState("");
  const [onlineDevices, setOnlineDevices] = useState({});
  const [connectStatus, setConnectStatus] = useState<keyof ConnectOptions>("disconnected");
  let staleTimeout: NodeJS.Timeout;
  let connectTimeout: NodeJS.Timeout;

  useEffect(() => {
    connectToBroker();
  }, []);

  function handleSubmit(e: React.FormEvent) {
    e.preventDefault();
    connectToBroker();
  }

  function connectToBroker() {
    clearTimeout(staleTimeout);
    clearTimeout(connectTimeout);
    connectTimeout = setTimeout(onConnectFail, 5000);
    setConnectStatus("connecting");
    setupMq(address, onSubscribe, onMessage, onHelloMessage);
  }

  function onSubscribe() {
    setConnectStatus("stale");
    clearTimeout(connectTimeout);
  }
  function onMessage() {
    clearTimeout(staleTimeout);
    if (connectStatus !== "connected") setConnectStatus("connected");
    staleTimeout = setTimeout(() => { setConnectStatus("stale") }, 2000);
  }

  interface Version {
    project: string;
    deviceId: number;
  };

  function onHelloMessage(versionData: Version) {
    const newDevice = versionData.project + "/" + versionData.deviceId;
    setOnlineDevices((prev) => ({...prev, [newDevice]: newDevice}))
  }
  function onConnectFail() {
    clearTimeout(staleTimeout);
    clearTimeout(connectTimeout);
    setConnectStatus("disconnected");
    teardownMq();
  }
  function onSelectDevice(e: React.ChangeEvent<HTMLSelectElement>) {
    selectDevice(e.target.value);
    setSelectedDevice(e.target.value);
  }

  return (
    <form onSubmit={handleSubmit}>
      {showBrokerUrl && (
        <label>
          Broker URL:
          <input type="text" value={address}
            onChange={(e) => setAddress(e.currentTarget.value)} />
          <br/>
        </label>
      )}
      <label>
        Device:
        <select
          name="device"
          value={selectedDevice}
          onChange={onSelectDevice}
        >
          <option value="" disabled>Select a device...</option>
          {Object.keys(onlineDevices).map((device) => (
            <option key={device} value={device}>{device}</option>
          ))}
        </select>
      </label>
      <span className={"pill " + statuses[connectStatus].colors}>
        {statuses[connectStatus].text}
      </span>
    </form>
  )
}