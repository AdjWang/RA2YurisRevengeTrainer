// Language configuration
const i18n = {
  'en': {
    loading: 'Web frontend loading...',
    connecting: 'Connecting...',
    connected: 'Connected! Loading application...',
    noUrl: 'No backend URL configured',
    notReachable: 'Web frontend disabled (backend not reachable)',
    connectionFailed: 'Web frontend disabled (connection failed)',
    loadFailed: 'Failed to load backend application',
    retry: 'Retry',
    retryAuto: 'Retrying automatically every 5 seconds...'
  },
  'zh': {
    loading: 'Web 前端加载中...',
    connecting: '正在连接...',
    connected: '已连接！正在加载应用...',
    noUrl: '未配置后端 URL',
    notReachable: 'Web 前端已禁用（后端无法访问）',
    connectionFailed: 'Web 前端已禁用（连接失败）',
    loadFailed: '加载后端应用失败',
    retry: '重试',
    retryAuto: '每 5 秒自动重试...'
  }
};

// Detect browser language
function getUserLanguage() {
  const lang = navigator.language || navigator.userLanguage;
  return lang.startsWith('zh') ? 'zh' : 'en';
}

// Get current language (default to browser language, but can be overridden)
let currentLanguage = getUserLanguage();

// Translation function
function t(key) {
  return i18n[currentLanguage][key] || i18n['en'][key] || key;
}

// Function to change language
function setLanguage(lang) {
  if (i18n[lang]) {
    currentLanguage = lang;
    // Update UI text if needed
    updateUIText();
  }
}

// Update UI text based on current language
function updateUIText() {
  const loadingContainer = document.getElementById('loading-container');
  if (loadingContainer && loadingContainer.style.display !== 'none') {
    const messageEl = loadingContainer.querySelector('.message:not(#status-message)');
    if (messageEl) messageEl.textContent = t('loading');
  }
}

// Pure JavaScript implementation of URL reachability check
async function checkUrlReachable(url, timeout = 2000) {
  const controller = new AbortController();
  const timeoutId = setTimeout(() => controller.abort(), timeout);

  try {
    // Use HEAD request to be lightweight, fallback to GET if HEAD not supported
    await fetch(url, {
      method: 'HEAD',
      signal: controller.signal,
      mode: 'no-cors' // This allows checking without CORS issues
    }).catch(() => fetch(url, {
      method: 'GET',
      signal: controller.signal,
      mode: 'no-cors'
    }));

    clearTimeout(timeoutId);

    // With no-cors, we can't access status, but if we get here, it's reachable
    return true;
  } catch (error) {
    clearTimeout(timeoutId);
    console.log('URL check failed:', error);
    return false;
  }
}

window.waitBackend = async function () {
  const err = window.errorMessage;
  if (err) {
    showBackendError(err, retry = false);
    return;
  }
  const url = window.backendUrl;
  const loadingContainer = document.getElementById('loading-container');
  const backendFrame = document.getElementById('backend-frame');
  const statusMessage = document.getElementById('status-message');
  if (!url) {
    showBackendError(t('noUrl'), retry = false);
    return;
  }
  try {
    statusMessage.textContent = t('connecting');

    // Check if backend is reachable
    const isReachable = await checkUrlReachable(url);

    if (isReachable) {
      statusMessage.textContent = t('connected');

      // Load the backend URL in iframe
      backendFrame.src = url;
      backendFrame.style.display = 'block';
      loadingContainer.style.display = 'none';

      // Handle iframe load error
      backendFrame.onerror = function () {
        showBackendError(t('loadFailed'), retry = true);
      };

      // Stop retry interval when connected
      stopRetryInterval();
    } else {
      showBackendError(t('notReachable'), retry = true);
    }
  } catch (error) {
    console.error('Error checking backend:', error);
    showBackendError(t('connectionFailed'), retry = true);
  }
};

function showBackendError(message, retry) {
  const loadingContainer = document.getElementById('loading-container');
  if (retry) {
    loadingContainer.innerHTML = `
          <div id="status-message" class="message error">${message}</div>
          <button class="retry-btn" onclick="window.waitBackend()">${t('retry')}</button>
          <div class="message" style="font-size: 12px; margin-top: 1rem;">
              ${t('retryAuto')}
          </div>
      `;
    // Ensure retry interval is running
    startRetryInterval();
  } else {
    loadingContainer.innerHTML = `
          <div id="status-message" class="message error">${message}</div>
      `;
  }
}

// Auto-retry every 5 seconds if failed
let retryInterval;

function startRetryInterval() {
  if (retryInterval) clearInterval(retryInterval);

  retryInterval = setInterval(() => {
    const backendFrame = document.getElementById('backend-frame');
    const loadingContainer = document.getElementById('loading-container');

    // Only retry if we're still in loading state (iframe not visible)
    if (backendFrame.style.display !== 'block' &&
      loadingContainer.style.display !== 'none') {
      window.waitBackend();
    }
  }, 5000);
}

function stopRetryInterval() {
  if (retryInterval) {
    clearInterval(retryInterval);
    retryInterval = null;
  }
}

// Start checking when page loads
window.addEventListener('load', () => {
  if (window.errorMessage || window.backendUrl) {
    updateUIText(); // Set initial loading text in Chinese if needed
    window.waitBackend();
    startRetryInterval();
  }
});

// Cleanup on unload
window.addEventListener('beforeunload', () => {
  stopRetryInterval();
});
