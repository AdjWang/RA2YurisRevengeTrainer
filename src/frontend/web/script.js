function initTab() {
  document.querySelectorAll('.tab').forEach(tab => {
    tab.addEventListener('click', () => {
      // Remove active class from all tabs and content
      document.querySelectorAll('.tab').forEach(t => t.classList.remove('active'));
      document.querySelectorAll('.tab-content').forEach(c => c.classList.remove('active'));

      // Add active class to clicked tab and corresponding content
      tab.classList.add('active');
      const tabId = tab.getAttribute('data-tab');
      document.getElementById(tabId).classList.add('active');
    });
  });
}

function initFilter() {
  // Source list functionality
  const sourceList = document.getElementById('source-list');
  const destinationList = document.getElementById('destination-list');
  const addAllBtn = document.getElementById('add-all-btn');
  const clearAllBtn = document.getElementById('clear-all-btn');

  // Add single item to destination list
  sourceList.querySelectorAll('.selectable').forEach(item => {
    item.addEventListener('click', () => {
      const clone = item.cloneNode(true);
      destinationList.appendChild(clone);

      // Make the cloned item removable
      clone.addEventListener('click', () => {
        destinationList.removeChild(clone);
      });
    });
  });

  // Add all items to destination list
  addAllBtn.addEventListener('click', () => {
    sourceList.querySelectorAll('.selectable').forEach(item => {
      // Check if item already exists in destination
      const exists = Array.from(destinationList.children).some(
        destItem => destItem.textContent === item.textContent
      );

      if (!exists) {
        const clone = item.cloneNode(true);
        destinationList.appendChild(clone);

        // Make the cloned item removable
        clone.addEventListener('click', () => {
          destinationList.removeChild(clone);
        });
      }
    });
  });

  // Clear all items from destination list
  clearAllBtn.addEventListener('click', () => {
    destinationList.innerHTML = '';
  });
}

function initButton() {
  document.getElementById('apply-btn').addEventListener('click', () => {
    const amount = document.getElementById('money-input').value;
    if (amount) {
      console.log(`Applied amount: ${amount}`);
    } else {
      console.log('Please enter an amount');
    }
  });
  document.querySelectorAll('#assist button:not(#apply-btn)').forEach(btn => {
    btn.addEventListener('click', () => {
      console.log(`Button "${btn.textContent}" clicked`);
    });
  });
}

function initCheckbox() {
  document.querySelectorAll('.checkbox-group input[type="checkbox"]').forEach(checkbox => {
    checkbox.addEventListener('change', (e) => {
      console.log(`Checkbox ${e.target.id} is now ${e.target.checked ? 'checked' : 'unchecked'}`);
    });
  });
}

document.addEventListener('DOMContentLoaded', function () {
  initTab();
  initFilter();
  initButton();
  initCheckbox();
});
